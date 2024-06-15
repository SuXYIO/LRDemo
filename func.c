#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//get weights & biases
extern double f_w;
extern double f_b;
extern double g_w;
extern double g_b;
//extern options
extern int a_func_num;
extern int l_func_num;

//declare getfunc functions
double (*get_a_func(int funcnum))(double);
double (*get_l_func(int funcnum))(double, double);
double (*get_agrad_func(int funcnum))(double);
double (*get_lgrad_func(int funcnum))(double, double, double);

//function pointers
double (*a_func)(double) = NULL;
double (*l_func)(double, double) = NULL;
double (*lgrad_func)(double, double, double) = NULL;
double (*agrad_func)(double) = NULL;

//calculate batch thread function
void* calc_batch(void* args)
{
	//get args
	void** arg = (void**) args;
	double* pl = arg[0];
	double* pgrad_w = arg[1];
	double* pgrad_b = arg[2];
	//calc
	double x = rand_nml(1.0, 1.0);
	double y_f = a_func(f(x));
	double y_g = a_func(g(x));
	//pass results
	*pl += l_func(y_g, y_f);
	*pgrad_w += lgrad_func(y_g, y_f, x) * agrad_func(x) * x;
	*pgrad_b += lgrad_func(y_g, y_f, x) * agrad_func(x);
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
double (*get_a_func(int funcnum))(double)
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
double (*get_l_func(int funcnum))(double, double)
{
	double (*func)(double, double);
	if (funcnum == 0)
		func = MSE;
	else
		func = NULL;
	return func;
}
double (*get_agrad_func(int funcnum))(double)
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
double (*get_lgrad_func(int funcnum))(double, double, double)
{
	double (*func)(double, double, double);
	if (funcnum == 0) {
		func = MSE_grad;
	} else {
		func = NULL;
	}
	return func;
}

//other function stuff
//test file for writing
int isusablefile(char* filename)
{
	FILE* filep = fopen(filename, "r");
	if (filep == NULL) {
		//file doesn't exist, probably
		return 0;
	} else {
		//file exists
		fclose(filep);
		printf("%sWARN: file already exists. \n%sFile: %s, in option '-f'. %s", COLOR_WARN, COLOR_END, filename, COLOR_END);
		bool input_valid = false;
		char usrinput = '\0';
		while (input_valid == false) {
			printf("\t\ny: yes; \t\nn: no, forget option '-f'; \t\nq: quit; \nOverwrite file? [y/n/q] > ");
			scanf("%c", &usrinput);
			if (usrinput == 'y') {
				//enable overwrite
				return 0;
				input_valid = true;
			} else if (usrinput == 'n') {
				//forget writetofile option
				return 1;
				input_valid = true;
			} else if (usrinput == 'q') {
				//quit
				return 2;
				input_valid = true;
			} else {
				//invalid option
				printf("Invalid option: \"%c\"", usrinput);
				input_valid = false;
			}
		}
	}
	return -1;
}
//print version
int printversion(void)
{
	printf("Name: LR\nVersion: %s\n", VER);
	return 0;
}
//show help (manual) page
int manualpage(void)
{
	char const manual_path[] = "./manual.txt";
	char const less_cmd[] = "less -R ";
	char cmd[sizeof(less_cmd) + sizeof(manual_path)] = "";
	sprintf(cmd, "%s%s", less_cmd, manual_path);
	int less_err = system(cmd);
	if (less_err != 0) {
		// less error
		FILE *file;
		char ch;
		file = fopen("example.txt", "r");
		if (file == NULL) {
			printf("%sError opening ./manual.txt. %sCheck if manual.txt exists. \n%s", COLOR_ERROR, COLOR_END, COLOR_END);
			return -1;
		}
		while ((ch = fgetc(file)) != EOF) {
			printf("%c", ch);
		}
		fclose(file);
	}
	return 0;
}
