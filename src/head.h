// LR (Linear Regression)
// By SuXYIO
// Created on 2024.05.18

#pragma once
#ifndef HEAD_H
#define HEAD_H
//defines
	//Version stuff
		#define VER "1.0.0"
	//Buffer size for strings
		#define STR_BUFSIZE 128
	//Float Precision for Print: how many digits to print after float decimal point
		#define FPP 4
	//ANSI color
		#define COLOR_NORM "\033[0m"
		#define COLOR_SUCC "\033[32m"
		#define COLOR_WARN "\033[33m"
		#define COLOR_ERROR "\033[31m"
		#define COLOR_END "\033[0m"
//typedefs
	//neuron
		typedef struct {
			double w;
			double b;
			double l;
			double wg;
			double bg;
		} neuron;
//functions
	//neuron.c
		double f(double x);
		double g(double x);
		double MSE(double e, double a);
		double MSE_grad(double e, double a);
		double None(double x);
		double grad(double x);
		double ReLU(double x);
		double ReLU_grad(double x);
		double LReLU(double x);
		double LReLU_grad(double x);
		double Sigmoid(double x);
		double Sigmoid_grad(double x);
		double Tanh(double x);
		double Tanh_grad(double x);
		double rand_noise(double factor);
		int strand(void);
		double rand_nml(double mean, double stddev);
		double rand_nmlstd(void);
	//func.c
		void* calc_batch(void* args);
		int init_n(void);
		int getfuncs(void);
		int isusablefile(char* filename);
		int printversion(void);
		int manualpage(void);
#endif
