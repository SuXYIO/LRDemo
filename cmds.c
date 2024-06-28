#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>

//get neurons
neuron nf;
neuron ng;
//extern options
extern int a_func_num;
extern int l_func_num;
extern int seed;
extern double eta;
extern double l_exp;
extern int batch_size;
extern int thread_size;
extern bool verbose;
extern bool use_thread;
extern bool writetofile;
extern char csvfilename[STR_BUFSIZE];
extern FILE* csvfilep;

//auto training
int auto_train(int const argc, char** const argv)
{
	//init
	seed = strand();
	//tmp var
	int o;
	const char* optstring = "Vts:f:A:L:b:e:l:vh";
	while ((o = getopt(argc, argv, optstring)) != -1) {
		switch (o) {
			case 'V':
				verbose = true;
				break;
			case 't':
				use_thread = true;
				break;
			case 's':
				seed = atoi(optarg);
				srand(atoi(optarg));
				break;
			case 'f':
				writetofile = true;
				strcpy(csvfilename, optarg);
				break;
			case 'A':
				a_func_num = atoi(optarg);
				break;
			case 'L':
				l_func_num = atoi(optarg);
				break;
			case 'b':
				batch_size = atoi(optarg);
				break;
			case 'e':
				eta = atof(optarg);
				break;
			case 'l':
				l_exp = atof(optarg);
				break;
			case '?':
				printf("%sError: invalid option: '%c'. %s\nUse \"./LR.out -h\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
		}
	}
	//init weights and biases with nml distro
	init_n();
	//record initial f_w & f_b for future use
	double nf_w_init = nf.w;
	double nf_b_init = nf.b;
	//init thread_size
	//prevent too many threads
	if (use_thread == true) {
		if (batch_size <= MAX_THREADS)
			thread_size = batch_size;
		else {
			thread_size = MAX_THREADS;
		}
	}
	//open file if -f is on
	//check if file is usable
	if (writetofile == true) {
		int usable = -1;
		usable = isusablefile(csvfilename);
		if (usable == 0) {
			//file usable
			writetofile = true;
		} else if (usable == 1) {
			//disable writetofile
			writetofile = false;
		} else if (usable == 2) {
			//quit
			return 0;
		} else {
			//error
			printf("%sFAIL: return of isusablefile() is invalid. \n%s", COLOR_FAIL, COLOR_END);
			return -1;
		}
	}
	//open file and write head
	if (writetofile == true) {
		csvfilep = fopen(csvfilename, "w");
		if (csvfilep == NULL) {
			printf("%sFAIL: error opening file. \n%s", COLOR_FAIL, COLOR_END);
			return -1;
		}
		fprintf(csvfilep, "f_w,f_b,g_w,g_b,l,nf.wg,nf.bg\n");
	}
	//get functions
	if (getfuncs() == -1) {
		printf("%sFAIL: error activation or loss function. \n%s", COLOR_FAIL, COLOR_END);
		return -1;
	}
	//count iteration
	int iter = 0;
	do {
		//calc batch
		if (use_thread == true) {
			//times of threads to run
			double tt = (float)batch_size / (float)thread_size;
			//executed thread number
			int exedt = 0;
			//use muti-thread to calculate batch
			pthread_t tid[thread_size];
			void* ret = NULL;
			for (float t = 0; t < tt; t++) {
				for (int i = 0; i < thread_size; i++)
					pthread_create(&tid[i], NULL, calc_batch, NULL);
				for (int i = 0; i < thread_size; i++) {
					pthread_join(tid[i], &ret);
					if (ret != NULL) {
						printf("%sFAIL: undefined activation function number. \n%s", COLOR_FAIL, COLOR_END);
						return -1;
					}
					exedt++;
				}
			}
			if (batch_size > exedt) {
				//remaining threads to be executed
				int rmnt = batch_size - exedt;
				for (int i = 0; i < rmnt; i++)
					pthread_create(&tid[i], NULL, calc_batch, NULL);
				for (int i = 0; i < rmnt; i++)
					pthread_join(tid[i], &ret);
				if (ret != NULL) {
					printf("%sFAIL: undefined activation function number. \n%s", COLOR_FAIL, COLOR_END);
					return -1;
				}
			}
		} else if (use_thread == false) {
			//normal calculate batch
			for (int i = 0; i < batch_size; i++) {
				calc_batch(NULL);
			}
			//normal calculate batch
			void* ret;
			for (int i = 0; i < batch_size; i++) {
				ret = calc_batch(NULL);
				if (ret != NULL) {
					printf("%sFAIL: undefined activation function number. \n%s", COLOR_FAIL, COLOR_END);
					return -1;
				}
			}
		}
		//calc loss & gradients (average of batch)
		nf.l /= batch_size;
		nf.wg /= batch_size;
		nf.bg /= batch_size;
		//update weights & biases
		nf.w -= eta * nf.wg;
		nf.b -= eta * nf.bg;
		//print results
		if (writetofile == true)
			fprintf(csvfilep, "%.*f,%.*f,%.*f,%.*f,%.*f,%.*f,%.*f\n", FPP, nf.w, FPP, nf.b, FPP, ng.w, FPP, ng.b, FPP, nf.l, FPP, nf.wg, FPP, nf.bg);
		if (verbose == true)
			printf("%siter = %d, nf.w = %.*f, nf.b = %.*f, ng.w = %.*f, ng.b = %.*f, nf.l = %.*f, nf.wg = %.*f, nf.bg = %.*f; \n%s", COLOR_NORM, iter, FPP, nf.w, FPP, nf.b, FPP, ng.w, FPP, ng.b, FPP, nf.l, FPP, nf.wg, FPP, nf.bg, COLOR_END);
		//check if gradient explosion
		if (isfinite(nf.l) != true || isfinite(nf.wg) != true || isfinite(nf.bg) != true) {
			printf("%sFAIL: l or nf.wg or nf.bg not finite, probably gradient explosion. \n%sseed = %d, \niter = %d, \nnf.w = %.*f, nf.b = %.*f, \nng.w = %.*f, ng.b = %.*f, \neta = %.*f, batch_size = %d, \nnf.l = %.*f, l_exp = %.*f\n%s", COLOR_FAIL, COLOR_NORM, seed, iter, FPP, nf.w, FPP, nf.b, FPP, ng.w, FPP, ng.b, FPP, eta, batch_size, FPP, nf.l, FPP, l_exp, COLOR_END);
			if (writetofile == true)
				fclose(csvfilep);
			return -1;
		}
		iter++;
	} while (nf.l >= l_exp);
	printf("%sSUCC: l >= l_exp. \n%sseed = %d, \niter = %d, \nnf.w_init = %.*f, nf.b_init = %.*f, \nnf.w = %.*f, nf.b = %.*f, \nng.w = %.*f, ng.b = %.*f, \neta = %.*f, batch_size = %d, \nnf.l = %.*f, l_exp = %.*f\n%s", COLOR_SUCC, COLOR_NORM, seed, iter, FPP, nf_w_init, FPP, nf_b_init, FPP, nf.w, FPP, nf.b, FPP, ng.w, FPP, ng.b, FPP, eta, batch_size, FPP, nf.l, FPP, l_exp, COLOR_END);
	if (writetofile == true)
		fclose(csvfilep);
	return 0;
}

//print version
int printversion(void)
{
	printf("Name: LR\nVersion: %s\n", VER);
	return 0;
}
//show help (manual) page
int manpage(char* entry_name)
{
	if (entry_name == NULL)
		entry_name = "LR";
	char* const manual_path = "./manual/";
	//use txt or lnk
	char path[STR_BUFSIZE] = "\0";
	sprintf(path, "%s%s.txt", manual_path, entry_name);
	if (isfileexist(path) == false) {
		//try lnk
		sprintf(path, "%s%s.lnk", manual_path, entry_name);
		if (isfileexist(path) == false) {
			printf("%sFAIL: fail opening manual. Fopen fail. \n%s", COLOR_FAIL, COLOR_END);
			return -1;
		}
	}
	//use less or printf
	if (system("less 2> /dev/null") != 0) {
		//less unusable, use print
		FILE* file = NULL;
		char ch;
		file = fopen(path, "r");
		while ((ch = fgetc(file)) != EOF) {
			printf("%c", ch);
		}
		fclose(file);
		return 0;
	} else {
		char cmd[STR_BUFSIZE] = "\0";
		sprintf(cmd, "less -R %s", path);
		if (system(cmd) != 0) {
			printf("%sFAIL: fail opening manual. Less command fail. \n%s", COLOR_FAIL, COLOR_END);
			return -1;
		}
	}
	return 0;
}
