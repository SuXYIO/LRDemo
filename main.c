#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

//weights & biases
	double f_w = 0.0;
	double f_b = 0.0;
	double g_w = 0.0;
	double g_b = 0.0;

int main(int const argc, char* const argv[]) {
	//check command args
		/*
		-q
			Quiet mode, only prints end info. 
		-v
			Print version info. 
		-h
			Help (manual) page, print help page and exit. 
		*/
		//store opts
		bool quiet = false;
		bool use_thread = false;
		bool use_seed = false;
		int seed = 0;
		//tmp var
		int o;
		const char* optstring = "qts:vh";
		while ((o = getopt(argc, argv, optstring)) != -1) {
			switch (o) {
				case 'q':
					quiet = true;
					break;
				case 't':
					use_thread = true;
					break;
				case 's':
					use_seed = true;
					seed = atoi(optarg);
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
					printf("%sError: invalid option: '%c'. %s\nUse \"./LR.out -h\" for help. \n%s", COLOR_ERROR, optopt, COLOR_END, COLOR_END);
					return -1;
					break;
			}
		}
	//init
		//seed rand
			if (use_seed == false)
				seed = strand();
		double l = 0.0;
		double grad_w = 0.0;
		double grad_b = 0.0;
		double eta = 0.0;
		double l_exp = 0.0;
		int batch_size = 0;
		int thread_size = 0;
	//init weights and biases with nml distro
		f_w = rand_nml(0.0, 1.0);
		f_b = rand_nml(0.0, 1.0);
		g_w = rand_nml(0.0, 1.0);
		g_b = rand_nml(0.0, 1.0);
	//set learning rate & loss expected & batch size
		eta = 0.001;
		l_exp = 0.0001;
		batch_size = 256;
	//record initial f_w & f_b for future use
		double f_w_init = f_w;
		double f_b_init = f_b;
	//init thread_size
	//prevent too many threads
		if (batch_size <= MAX_THREADS)
			thread_size = batch_size;
		else
			thread_size = MAX_THREADS;
	//count iteration
		int iter = 0;
	do {
		//init (just in case)
		l = 0.0;
		grad_w = 0.0;
		grad_b = 0.0;
		//calc batch
		if (use_thread == true) {
			//use muti-thread to calculate batch
			pthread_t tid[thread_size];
			void* args[3] = {&l, &grad_w, &grad_b};
			for (int i = 0; i < thread_size; i++)
				pthread_create(&tid[i], NULL, calc_batch, args);
			for (int i = 0; i < thread_size; i++)
				pthread_join(tid[i], NULL);
		} else if (use_thread == false) {
			//normal calculate batch
			void* args[3] = {&l, &grad_w, &grad_b};
			for (int i = 0; i < thread_size; i++)
				calc_batch(args);
		}
		//calc loss & gradients (average of batch)
		l /= batch_size;
		grad_w /= batch_size;
		grad_b /= batch_size;
		//update weights & biases
		f_w -= eta * grad_w;
		f_b -= eta * grad_b;
		//print results
		if (quiet == false)
			printf("%siter = %d, f_w = %.*f, f_b = %.*f, g_w = %.*f, g_b = %.*f, l = %.*f, grad_w = %.*f, grad_b = %.*f; \n%s", COLOR_END, iter, FPP, f_w, FPP, f_b, FPP, g_w, FPP, g_b, FPP, l, FPP, grad_w, FPP, grad_b, COLOR_END);
		//check if gradient explosion
		if (isfinite(l) != true || isfinite(grad_w) != true || isfinite(grad_b) != true) {
			printf("%sERROR: l or grad_w or grad_b not finite, probably gradient explosion. \n%siter = %d, \nf_w = %.*f, f_b = %.*f, \ng_w = %.*f, g_b = %.*f, \neta = %.*f, batch_size = %d, \nl = %.*f, l_exp = %.*f\n%s", COLOR_ERROR, COLOR_END, iter, FPP, f_w, FPP, f_b, FPP, g_w, FPP, g_b, FPP, eta, batch_size, FPP, l, FPP, l_exp, COLOR_END);
			return -1;
		}
		iter++;
	} while (l >= l_exp);
	printf("%sSUCCESS: l >= l_exp. \n%sseed = %d, \niter = %d, \nf_w_init = %.*f, f_b_init = %.*f, \nf_w = %.*f, f_b = %.*f, \ng_w = %.*f, g_b = %.*f, \neta = %.*f, batch_size = %d, \nl = %.*f, l_exp = %.*f\n%s", COLOR_SUCC, COLOR_END, seed, iter, FPP, f_w_init, FPP, f_b_init, FPP, f_w, FPP, f_b, FPP, g_w, FPP, g_b, FPP, eta, batch_size, FPP, l, FPP, l_exp, COLOR_END);
	return 0;
}
