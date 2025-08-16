#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

//neuron f()
neuron nf;
//training neuron g()
neuron ng;
//extern options
int a_func_num = 0;
int l_func_num = 0;
double noise_factor = 0.0;

int main(int const argc, char* const argv[])
{
	//init
	int seed = 0;
	//seed rand
	seed = strand();
	double eta = 0.001;
	unsigned long max_epochs = 0;
	double l_exp = 0.001;
	unsigned int batch_size = 256;
	//check command args
	//store opts
	bool verbose = false;
	bool writetofile = false;
	char csvfilename[STR_BUFSIZE];
	FILE* csvfilep = NULL;
	//tmp var
	int o;
	const char* optstring = "Vs:f:A:L:E:b:e:l:n:vh";
	while ((o = getopt(argc, argv, optstring)) != -1) {
		switch (o) {
			case 'V':
				verbose = true;
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
			case 'E':
				max_epochs = atoi(optarg);
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
			case 'n':
				noise_factor = atof(optarg);
				break;
			case 'v':
				printversion();
				return 0;
				break;
			case 'h':
				manualpage();
				return 0;
				break;
			case '?':
				printf("%sError: invalid option: '%c'. %s\nUse \"./LR -h\" for help. \n%s", COLOR_ERROR, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
		}
	}
	//init weights and biases with nml distro
	init_n();
	//record initial f_w & f_b for future use
	double nf_w_init = nf.w;
	double nf_b_init = nf.b;
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
			printf("%sERROR: return of isusablefile() is invalid. \n%s", COLOR_ERROR, COLOR_END);
			return -1;
		}
	}
	//open file and write head
	if (writetofile == true) {
		csvfilep = fopen(csvfilename, "w");
		if (csvfilep == NULL) {
			printf("%sERROR: error opening file. \n%s", COLOR_ERROR, COLOR_END);
			return -1;
		}
		fprintf(csvfilep, "f_w,f_b,g_w,g_b,l,nf.wg,nf.bg\n");
	}
	//get functions
	if (getfuncs() == -1) {
		printf("%sERROR: error activation or loss function. \n%s", COLOR_ERROR, COLOR_END);
		return -1;
	}
	//count iteration
	int iter = 0;
	do {
		//calc batch
		/*
		// this piece of code is abandoned, since the multithread didn't work out to improve performance
		// the argument 't' is also deleted
		if (thread_size != 0) {
			//use multithread
			//times of threads to run
			double tt = (float)batch_size / (float)thread_size;
			//executed thread number
			int exedt = 0;
			//use muti-thread to calculate batch
			pthread_t tid[thread_size];
			for (float t = 0; t < tt; t++) {
				for (int i = 0; i < thread_size; i++)
					pthread_create(&tid[i], NULL, calc_batch, NULL);
				for (int i = 0; i < thread_size; i++) {
					pthread_join(tid[i], NULL);
					exedt++;
				}
			}
			if (batch_size > exedt) {
				//remaining threads to be executed
				int rmnt = batch_size - exedt;
				for (int i = 0; i < rmnt; i++)
					pthread_create(&tid[i], NULL, calc_batch, NULL);
				for (int i = 0; i < rmnt; i++)
					pthread_join(tid[i], NULL);
			}
		} else {
			//normal calculate batch
			for (int i = 0; i < batch_size; i++) {
				calc_batch(NULL);
			}
		}
		*/
		// calculate batch
		for (int i = 0; i < batch_size; i++) {
			calc_batch(NULL);
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
			printf("%sERROR: l or nf.wg or nf.bg not finite, probably gradient explosion. \n%sseed = %d, \niter = %d, \nnf.w = %.*f, nf.b = %.*f, \nng.w = %.*f, ng.b = %.*f, \neta = %.*f, batch_size = %d, \nnf.l = %.*f, l_exp = %.*f, noise_factor = %.*f\n%s", COLOR_ERROR, COLOR_NORM, seed, iter, FPP, nf.w, FPP, nf.b, FPP, ng.w, FPP, ng.b, FPP, eta, batch_size, FPP, nf.l, FPP, l_exp, FPP, noise_factor, COLOR_END);
			if (writetofile == true)
				fclose(csvfilep);
			return -1;
		}
		iter++;

		//check for success
		if (max_epochs != 0) {
			// use epoch (iter) check
			if (iter >= max_epochs)
				break;
		} else {
			// use loss check
			if (nf.l < l_exp)
				break;
		}
	} while (true);
	char* succ_msg;
	if (max_epochs != 0) {
		succ_msg = "epoch >= max_epochs";
	} else {
		succ_msg = "l >= l_exp";
	}
	printf("%sSUCC: %s. \n%sseed = %d, \niter = %d, \nnf.w_init = %.*f, nf.b_init = %.*f, \nnf.w = %.*f, nf.b = %.*f, \nng.w = %.*f, ng.b = %.*f, \neta = %.*f, batch_size = %d, \nnf.l = %.*f, l_exp = %.*f, noise_factor = %.*f\n%s", COLOR_SUCC, succ_msg, COLOR_NORM, seed, iter, FPP, nf_w_init, FPP, nf_b_init, FPP, nf.w, FPP, nf.b, FPP, ng.w, FPP, ng.b, FPP, eta, batch_size, FPP, nf.l, FPP, l_exp, FPP, noise_factor, COLOR_END);
	if (writetofile == true)
		fclose(csvfilep);
	return 0;
}
