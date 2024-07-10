#include "main.h"
#include <stdbool.h>
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
extern int ret;

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
				if (strcmp(optarg, "a") == 0) {
					i_f = true;
					i_g = true;
				} else if (strcmp(optarg, "f") == 0)
					i_f = true;
				else if (strcmp(optarg, "g") == 0)
					i_g = true;
				else {
					printf("%sFAIL: invalid option: '-%c %s'. %s\nUse \"h i\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
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
					printf("%sFAIL: invalid option: '-%c %s'. %s\nUse \"h i\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
					return -1;
				}
				break;
			case '?':
				printf("%sFAIL: invalid option: '-%c'. %s\nUse \"h i\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sFAIL: unknown option: '-%c'. %s\nUse \"h i\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
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

//print values
int print(int const argc, char** const argv)
{
	char fmtstr[STR_BUFSIZE] = "\0";
	//tmp var
	int o = '?';
	int tmp = 0;
	optreset = true;
	optind = 1;
	while ((o = getopt(argc, argv, "v:")) != -1) {
		switch (o) {
			case 'v':
				strcpy(fmtstr, optarg);
				break;
			case '?':
				printf("%sFAIL: invalid option: '-%c'. %s\nUse \"h p\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sFAIL: unknown option: '-%c'. %s\nUse \"h p\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
				return -1;
		}
	}
	tmp = strlen(fmtstr);
			for (int i = 0; i < tmp; i++) {
				if (fmtstr[i] == '%') {
					i++;
					if (fmtstr[i] == 's')
						printf("%d", seed);
					else if (fmtstr[i] == 'r')
						printf("%d", ret);
					else if (fmtstr[i] == '%')
						printf("%%");
					else {
						printf("%s\nFAIL: invalid option: '-%c %s'. %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
						return 1;
					}
				} else
					printf("%c", fmtstr[i]);
			}
	printf("\n");
	return 0;
}

//print values for neuron
//TODO: redesign to support mutineuron outputting
int printn(int const argc, char** const argv)
{
	char fmtstr[STR_BUFSIZE] = "%w, %b, %l, %W, %B\n";
	bool tofile = false;
	bool p_f = true;
	bool p_g = true;
	FILE* fp = NULL;
	//local floating point precision
	int lfpp = FPP;
	//tmp var
	int o = '?';
	int tmp = 0;
	optreset = true;
	optind = 1;
	while ((o = getopt(argc, argv, "n:f:F:p:")) != -1) {
		switch (o) {
			case 'n':
				if (strcmp(optarg, "f") == 0) {
					p_f = true;
					p_g = false;
				} else if (strcmp(optarg, "g") == 0) {
					p_f = false;
					p_g = true;
				} else if (strcmp(optarg, "a") == 0) {
					p_f = true;
					p_g = true;
				} else {
					printf("%sFAIL: invalid option: '-%c %s'. %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
					return 1;
				}
				break;
			case 'f':
				strcpy(fmtstr, optarg);
				break;
			case 'F':
				tofile = true;
				short t = isusablefile(optarg);
				if (t == 0)
					fp = fopen(optarg, "w");
				else if (t == 1)
					tofile = false;
				else if (t == 2)
					return 1;
				else {
					printf("%sFAIL: isusablefile() fail. \n%s", COLOR_FAIL, COLOR_END);
					return 1;
				}
				break;
			case 'p':
				lfpp = atoi(optarg);
				break;
			case '?':
				printf("%sFAIL: invalid option: '-%c'. %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sFAIL: unknown option: '-%c'. %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
				return -1;
		}
	}
	short dotime = 1;
	tmp = strlen(fmtstr);
	neuron* npt = NULL;
	if (p_f == true)
		npt = &nf;
	if (p_g == true)
		npt = &ng;
	if (p_f == true && p_g == true)
		dotime = 2;
	for (int i = 0; i < dotime; i++) {
		if (dotime == 2) {
			if (i == 0)
				npt = &nf;
			else if (i == 1)
				npt = &ng;
		}
		if (tofile == false) {
			for (int i = 0; i < tmp; i++) {
				if (fmtstr[i] == '%') {
					i++;
					if (fmtstr[i] == 'w')
						printf("%.*f", lfpp, (*npt).w);
					else if (fmtstr[i] == 'b')
						printf("%.*f", lfpp, (*npt).b);
					else if (fmtstr[i] == 'l')
						printf("%.*f", lfpp, (*npt).l);
					else if (fmtstr[i] == 'W')
						printf("%.*f", lfpp, (*npt).wg);
					else if (fmtstr[i] == 'B')
						printf("%.*f", lfpp, (*npt).bg);
					else if (fmtstr[i] == '%')
						printf("%%");
					else {
						printf("%s\nFAIL: invalid option: '-%c %s'. %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
						return 1;
					}
				} else
					printf("%c", fmtstr[i]);
			}
		} else if (tofile == true) {
			for (int i = 0; i < tmp; i++) {
				if (fmtstr[i] == '%') {
					i++;
					if (fmtstr[i] == 'w')
						fprintf(fp, "%.*f", lfpp, (*npt).w);
					else if (fmtstr[i] == 'b')
						fprintf(fp, "%.*f", lfpp, (*npt).b);
					else if (fmtstr[i] == 'l')
						fprintf(fp, "%.*f", lfpp, (*npt).l);
					else if (fmtstr[i] == 'W')
						fprintf(fp, "%.*f", lfpp, (*npt).wg);
					else if (fmtstr[i] == 'B')
						fprintf(fp, "%.*f", lfpp, (*npt).bg);
					else if (fmtstr[i] == '%')
						fprintf(fp, "%%");
					else {
						printf("%s\nFAIL: invalid option: '-%c %s' (file written). %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
						return 1;
					}
				} else
					fprintf(fp, "%c", fmtstr[i]);
			}
		}
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
					printf("%sFAIL: option '-%c' requires an argument. %s\nUse \"h sr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				srand(atoi(optarg));
				seed = atoi(optarg);
				return 0;
				break;
			case '?':
				printf("%sFAIL: invalid option: '-%c'. %s\nUse \"h sr\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sFAIL: unknown option: '-%c'. %s\nUse \"h sr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
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
					printf("%sFAIL: option '-%c' requires an argument. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				writetofile = true;
				strcpy(csvfilename, optarg);
				break;
			case 'A':
				if (optarg == NULL) {
					printf("%sFAIL: option '-%c' requires an argument. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				a_func_num = atoi(optarg);
				break;
			case 'L':
				if (optarg == NULL) {
					printf("%sFAIL: option '-%c' requires an argument. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				l_func_num = atoi(optarg);
				break;
			case 'b':
				if (optarg == NULL) {
					printf("%sFAIL: option '-%c' requires an argument. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				batch_size = atoi(optarg);
				break;
			case 'e':
				if (optarg == NULL) {
					printf("%sFAIL: option '-%c' requires an argument. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				eta = atof(optarg);
				break;
			case 'l':
				if (optarg == NULL) {
					printf("%sFAIL: option '-%c' requires an argument. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				l_exp = atof(optarg);
				break;
			case '?':
				printf("%sFAIL: invalid option: '-%c'. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sFAIL: unknown option: '-%c'. %s\nUse \"h tr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
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
int manpage(int const argc, char** const argv)
{
	bool useraw = false;
	//tmp var
	int o = '?';
	optreset = true;
	if (argc > 1 && argv[1][0] != '-')
		optind = 2;
	else if (argc > 1 && argv[1][0] == '-')
		optind = 1;
	else
		optind = 1;
	while ((o = getopt(argc, argv, "r")) != -1) {
		switch (o) {
			case 'r':
				useraw = true;
				break;
			case '?':
				printf("%sFAIL: invalid option: '-%c'. %s\nUse \"h h\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sFAIL: unknown option: '-%c'. %s\nUse \"h h\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
				return -1;
		}
	}
	char filename[STR_BUFSIZE] = "\0";
	if (argc <= 1 || isemptystr(argv[1]) || argv[1][0] == '-') {
		strcpy(filename, "LR");
	} else {
		strcpy(filename, argv[1]);
	}
	char* const manual_path = "./manual/";
	//use txt or lnk
	char path[STR_BUFSIZE] = "\0";
	sprintf(path, "%s%s.txt", manual_path, filename);
	if (isfileexist(path) == false) {
		//try lnk
		sprintf(path, "%s%s.lnk", manual_path, filename);
		if (isfileexist(path) == false) {
			printf("%sFAIL: fail opening manual. Fopen fail. \n%s", COLOR_FAIL, COLOR_END);
			return -1;
		}
	}
	//use less or printf
	if (useraw == true || system("less 2> /dev/null") != 0) {
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
