// LR (Linear Regression)
// Version 1.0.0
// By SuXY
// Created on 2024.05.18

#pragma once
#ifndef HEAD_H
#define HEAD_H
//defines
	//Float Precision for Print: how many digits to print after float decimal point
		#define FPP 4
	//ANSI color
		#define COLOR_NORM "\033[0m"
		#define COLOR_SUCC "\033[32m"
		#define COLOR_ERROR "\033[31m"
		#define COLOR_END "\033[0m"
//functions
	//neuron.c
		double f(double x);
		double g(double x);
		double MSE(double e, double a);
		double MSE_grad_w(double e, double x);
		double MSE_grad_b(double e, double x);
		int strand(void);
		double rand_nml(double mean, double stddev);
#endif
