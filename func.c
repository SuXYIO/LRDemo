#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//get weights & biases
extern double f_w;
extern double f_b;
extern double g_w;
extern double g_b;

//declare getfunc functions
double (*get_a_func(int funcnum))(double);
double (*get_grad_w_func(int funcnum))(double, double, double);
double (*get_grad_b_func(int funcnum))(double, double, double);

//function pointers
double (*a_func)(double) = NULL;
double (*l_func)(double, double) = NULL;
double (*grad_w_func)(double, double, double) = NULL;
double (*grad_b_func)(double, double, double) = NULL;

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
	*pgrad_w += grad_w_func(y_g, y_f, x);
	*pgrad_b += grad_b_func(y_g, y_f, x);
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
int getfuncs(int funcnum)
{
	a_func = get_a_func(funcnum);
	grad_w_func = get_grad_w_func(funcnum);
	grad_b_func = get_grad_b_func(funcnum);
	l_func = MSE;
	return 0;
}
double (*get_a_func(int funcnum))(double)
{
	double (*a_func)(double);
	if (funcnum == 0) {
		a_func = None;
	} else if (funcnum == 1) {
		a_func = ReLU;
	} else if (funcnum == 2) {
		a_func = LReLU;
	} else if (funcnum == 3) {
		a_func = Sigmoid;
	} else if (funcnum == 4) {
		a_func = Tanh;
	} else {
		a_func = None;
	}
	return a_func;
}
double (*get_grad_w_func(int funcnum))(double, double, double)
{
	double (*grad_w_func)(double, double, double);
	if (funcnum == 0) {
		grad_w_func = MSE_grad_w;
	} else if (funcnum == 1) {
		grad_w_func = MSE_grad_w_ReLU;
	} else if (funcnum == 2) {
		grad_w_func = MSE_grad_w_LReLU;
	} else if (funcnum == 3) {
		grad_w_func = MSE_grad_w_Sigmoid;
	} else if (funcnum == 4) {
		grad_w_func = MSE_grad_w_Tanh;
	} else {
		grad_w_func = MSE_grad_w;
	}
	return grad_w_func;
}
double (*get_grad_b_func(int funcnum))(double, double, double)
{
	double (*grad_b_func)(double, double, double);
	if (funcnum == 0) {
		grad_b_func = MSE_grad_w;
	} else if (funcnum == 1) {
		grad_b_func = MSE_grad_w_ReLU;
	} else if (funcnum == 2) {
		grad_b_func = MSE_grad_w_LReLU;
	} else if (funcnum == 3) {
		grad_b_func = MSE_grad_w_Sigmoid;
	} else if (funcnum == 4) {
		grad_b_func = MSE_grad_w_Tanh;
	} else {
		grad_b_func = MSE_grad_w;
	}
	return grad_b_func;
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
