// LR (Linear Regression)
// By SuXYIO
// Created on 2024.05.18

#pragma once
#ifndef MAIN_H
#define MAIN_H
//includes
#include <stdbool.h>
//defines
	//Version stuff
		#define VER "2.0.0"
	//path to manual
		#define MANPATH "./manual/"
	//Max number of threads to run
		#define MAX_THREADS 1024
	//Buffer size for strings
		#define STR_BUFSIZE 256
	//Interface stuff
		#define PROMPT "> "
		#define ENDLINE '\n'
		#define MAXARGS 64
		#define CMDSEP " \t\n"
		#define CMDHISTSIZE 16
	//Float Precision for Print: how many digits to print after float decimal point
		#define FPP 4
	//ANSI color
		#define COLOR_NORM "\033[0m"
		#define COLOR_SUCC "\033[32m"
		#define COLOR_WARN "\033[33m"
		#define COLOR_FAIL "\033[31m"
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
	//neuronmath.c
		double f(double);
		double g(double);
		double MSE(double, double);
		double MSE_grad(double, double);
		double None(double);
		double grad(double);
		double ReLU(double);
		double ReLU_grad(double);
		double LReLU(double);
		double LReLU_grad(double);
		double Sigmoid(double);
		double Sigmoid_grad(double);
		double Tanh(double);
		double Tanh_grad(double);
		int strand(void);
		double rand_nml(double, double);
		double rand_nmlstd(void);
		double r0(void);
	//calcfunc.c
		void* calc_batch(void*);
		int getfuncs(void);
	//isfunc.c
		bool isusablefile(char* const);
		bool isfileexist(char* const);
		bool isfileexist(char* const);
		bool isemptystr(char* const);
		bool msc(char*, int, ...);
	//iofunc.c
		char* inputline(char*);
		int toargs(void);
	//./cmds
		int init(int, char**);
		int print(int, char**);
		int printn(int, char**);
		int seedrand(int, char**);
		int train(int, char**);
		int printversion(void);
		int manpage(int, char**);
		int enabledebug(int, char**);
		int disabledebug(int, char**);
		//debug cmds
		int printstr(int, char**);
#endif
