#include "main.h"
#include <stdio.h>

//get neurons
neuron nf;
neuron ng;
//extern options
extern short a_func_num;
extern short l_func_num;

//declare getfunc functions
double (*get_a_func(short))(double);
double (*get_l_func(short))(double, double);
double (*get_agrad_func(short))(double);
double (*get_lgrad_func(short))(double, double);

//function pointers
double (*a_func)(double) = NULL;
double (*l_func)(double, double) = NULL;
double (*agrad_func)(double) = NULL;
double (*lgrad_func)(double, double) = NULL;

//calculate batch thread function
void* calc_batch(void* args)
{
	//calc
	double x = rand_nmlstd();
	double y_f = a_func(f(x));
	double y_g = a_func(g(x));
	//pass results
	nf.l += l_func(y_g, y_f);
	nf.wg += lgrad_func(y_g, y_f) * agrad_func(x) * x;
	nf.bg += lgrad_func(y_g, y_f) * agrad_func(x);
	return NULL;
}

//get function pointers
/*
	functionnum:
		0: None
		1: ReLU
		2: LeakyReLU
		3: Sigmoid
		4: Tanh
*/
int getfuncs(void)
{
	a_func = get_a_func(a_func_num);
	agrad_func = get_agrad_func(a_func_num);
	l_func = get_l_func(l_func_num);
	lgrad_func = get_lgrad_func(l_func_num);
	if (a_func == NULL || agrad_func == NULL || lgrad_func == NULL || l_func == NULL)
		return -1;
	return 0;
}
double (*get_a_func(short funcnum))(double)
{
	double (*func)(double);
	if (funcnum == 0)
		func = None;
	else if (funcnum == 1)
		func = ReLU;
	else if (funcnum == 2)
		func = LReLU;
	else if (funcnum == 3)
		func = Sigmoid;
	else if (funcnum == 4)
		func = Tanh;
	else
		func = NULL;
	return func;
}
double (*get_l_func(short funcnum))(double, double)
{
	double (*func)(double, double);
	if (funcnum == 0)
		func = MSE;
	else
		func = NULL;
	return func;
}
double (*get_agrad_func(short funcnum))(double)
{
	double (*func)(double);
	if (funcnum == 0)
		func = grad;
	else if (funcnum == 1)
		func = ReLU_grad;
	else if (funcnum == 2)
		func = LReLU_grad;
	else if (funcnum == 3)
		func = Sigmoid_grad;
	else if (funcnum == 4)
		func = Tanh_grad;
	else
		func = NULL;
	return func;
}
double (*get_lgrad_func(short funcnum))(double, double)
{
	double (*func)(double, double);
	if (funcnum == 0) {
		func = MSE_grad;
	} else {
		func = NULL;
	}
	return func;
}
