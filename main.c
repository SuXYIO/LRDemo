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
		//tmp var
		int o;
		const char* optstring = "qvh";
		while ((o = getopt(argc, argv, optstring)) != -1) {
			switch (o) {
				case 'q':
					quiet = true;
					break;
				case 'v':
					printversion();
					return 0;
					break;
				case 'h':
					manualpage();
					return 0;
					break;
			}
		}
	//init
		double x = 0.0;
		double y_f = 0.0;
		double y_g = 0.0;
		double l = 0.0;
		double grad_w = 0.0;
		double grad_b = 0.0;
		double eta = 0.0;
		double l_exp = 0.0;
		int batch_size = 0;
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
	//seed rand
		strand();
	//count iteration
		int iter = 0;
	do {
		//calc batch
		for (int i = 0; i < batch_size; i++) {
			x = rand_nml(1.0, 1.0);
			y_f = f(x);
			y_g = g(x);
			l += MSE(y_g, y_f);
			grad_w += MSE_grad_w(y_g, x);
			grad_b += MSE_grad_b(y_g, x);
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
	printf("%sSUCCESS: l >= l_exp. \n%siter = %d, \nf_w_init = %.*f, f_b_init = %.*f, \nf_w = %.*f, f_b = %.*f, \ng_w = %.*f, g_b = %.*f, \neta = %.*f, batch_size = %d, \nl = %.*f, l_exp = %.*f\n%s", COLOR_SUCC, COLOR_END, iter, FPP, f_w_init, FPP, f_b_init, FPP, f_w, FPP, f_b, FPP, g_w, FPP, g_b, FPP, eta, batch_size, FPP, l, FPP, l_exp, COLOR_END);
	return 0;
}
