#include "head.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

//get weights & biases
	extern double f_w;
	extern double f_b;
	extern double g_w;
	extern double g_b;

//Functions
double f(double x) {
	return f_w * x + f_b;
}
double g(double x) {
	return g_w * x + g_b;
}

//MSE & MSE gradient
double MSE(double e, double a) {
	double l;
	l = pow(e - a, 2);
	return l;
}
double MSE_grad_w(double e, double x) {
	double grad_w;
	grad_w = 2.0*x*(f_w*x + f_b - e);
	return grad_w;
}
double MSE_grad_b(double e, double x) {
	double grad_b;
	grad_b = 2.0*(f_w*x + f_b - e);
	return grad_b;
}

//seed random using sec * nsec
int strand(void) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	int seed = ts.tv_nsec * time(NULL);
	srand(seed);
	return seed;
}

//Normal distro random
double rand_uniform() {
	return (double) rand() / RAND_MAX;
}
double box_muller() {
	static double r[2];
	double u1, u2, r1, r2;
	do {
		u1 = rand_uniform();
		u2 = rand_uniform();
		r1 = 2.0 * u1 - 1.0;
		r2 = 2.0 * u2 - 1.0;
	} while (r1 * r1 + r2 * r2 > 1.0);
	double f = sqrt(-2.0 * log(u1));
	r[0] = r1 * f;
	r[1] = r2 * f;
	return r[0];
}
double rand_nml(double mean, double stddev) {
	double r;
	r = box_muller();
	return r * stddev + mean;
}
