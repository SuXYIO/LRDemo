#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <pthread.h>

//get neurons
neuron nf;
neuron ng;
//extern options
extern int a_func_num;
extern int l_func_num;
extern int seed;

//init neurons
int init(int const argc, char** const argv)
{
	bool i_f = true;
	bool i_g = true;
	double (*ifunc)(void) = rand_nmlstd;
	//tmp var
	int o = '?';
	int tmp = 1;
	optreset = true;
	optind = 1;
	while ((o = getopt(argc, argv, "n:m:")) != -1) {
		switch (o) {
			case 'n':
				if (optarg[0] == 'a') {
					i_f = true;
					i_g = true;
				} else if (optarg[0] == 'f')
					i_f = true;
				else if (optarg[0] == 'g')
					i_g = true;
				else {
					printf("%sError: invalid option: '-%c %s'. %s\nUse \"h i\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
					return -1;
				}
				break;
			case 'm':
				tmp = atoi(optarg);
				if (tmp == 0)
					ifunc = r0;
				else if (tmp == 1)
					ifunc = rand_nmlstd;
				else {
					printf("%sError: invalid option: '-%c %s'. %s\nUse \"h i\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
					return -1;
				}
				break;
			case '?':
				printf("%sError: invalid option: '-%c'. %s\nUse \"h i\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sError: unknown option: '-%c'. %s\nUse \"h i\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
				return -1;
		}
	}
	if (i_f == true) {
		//neuron f
		nf.w = ifunc();
		nf.b = ifunc();
		nf.l = 0.0;
		nf.wg = 0.0;
		nf.bg = 0.0;
	}
	if (i_g == true) {
		//neuron g
		ng.w = ifunc();
		ng.b = ifunc();
	}
	return 0;
}

//seed random
int seedrand(int const argc, char** const argv)
{
	//tmp var
	int o = '?';
	optreset = true;
	optind = 1;
	while ((o = getopt(argc, argv, "s:")) != -1) {
		switch (o) {
			case 's':
				if (optarg == NULL) {
					printf("%sError: option '-%c' requires an argument. %s\nUse \"h sr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				srand(atoi(optarg));
				seed = atoi(optarg);
				return 0;
				break;
			case '?':
				printf("%sError: invalid option: '-%c'. %s\nUse \"h sr\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sError: unknown option: '-%c'. %s\nUse \"h sr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
				return -1;
		}
	}
	seed = strand();
	return 0;
}

//auto training
int train(int const argc, char** const argv)
{
	double eta = 0.001;
	double l_exp = 0.0001;
	int batch_size = 256;
	int thread_size = 0;
	bool verbose = false;
	bool use_thread = false;
	bool writetofile = false;
	char csvfilename[STR_BUFSIZE];
	FILE* csvfilep = NULL;
	//tmp var
	int o = '?';
	optreset = true;
	optind = 1;
	while ((o = getopt(argc, argv, "Vtf:A:L:b:e:l:")) != -1) {
		switch (o) {
			case 'V':
				verbose = true;
				break;
			case 't':
				use_thread = true;
				break;
			case 'f':
				if (optarg == NULL) {
					printf("%sError: option '-%c' requires an argument. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				writetofile = true;
				strcpy(csvfilename, optarg);
				break;
			case 'A':
				if (optarg == NULL) {
					printf("%sError: option '-%c' requires an argument. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				a_func_num = atoi(optarg);
				break;
			case 'L':
				if (optarg == NULL) {
					printf("%sError: option '-%c' requires an argument. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				l_func_num = atoi(optarg);
				break;
			case 'b':
				if (optarg == NULL) {
					printf("%sError: option '-%c' requires an argument. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				batch_size = atoi(optarg);
				break;
			case 'e':
				if (optarg == NULL) {
					printf("%sError: option '-%c' requires an argument. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				eta = atof(optarg);
				break;
			case 'l':
				if (optarg == NULL) {
					printf("%sError: option '-%c' requires an argument. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				l_exp = atof(optarg);
				break;
			case '?':
				printf("%sError: invalid option: '-%c'. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sError: unknown option: '-%c'. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
				return -1;
		}
	}
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
