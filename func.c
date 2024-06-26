#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

//get neurons
neuron nf;
neuron ng;
//extern options
extern int a_func_num;
extern int l_func_num;

//declare getfunc functions
double (*get_a_func(int funcnum))(double);
double (*get_l_func(int funcnum))(double, double);
double (*get_agrad_func(int funcnum))(double);
double (*get_lgrad_func(int funcnum))(double, double);

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

//init neurons
int init_n(void)
{
	double (*randfunc)(void) = rand_nmlstd;
	//neuron f
	nf.w = randfunc();
	nf.b = randfunc();
	nf.l = 0.0;
	nf.wg = 0.0;
	nf.bg = 0.0;
	//neuron g
	ng.w = randfunc();
	ng.b = randfunc();
	return 0;
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
double (*get_lgrad_func(int funcnum))(double, double)
{
	double (*func)(double, double);
	if (funcnum == 0) {
		func = MSE_grad;
	} else {
		func = NULL;
	}
	return func;
}

//other function stuff
//test file for writing
bool isusablefile(char* const filename)
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
bool isfileexist(char* const filename)
{
	FILE* filep = fopen(filename, "r");
	if (filep == NULL)
		return false;
	fclose(filep);
	return true;
}
char* inputline(char* prompt)
{
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO | ECHOE);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	//init
	static char input[STR_BUFSIZE] = "\0";
	for (int i = 0; i < STR_BUFSIZE; i++)
		input[i] = '\0';
	//distance to prompt
	int dp = 0;
	int i = 0;
	int len = 0;
	printf("%s", prompt);
	fflush(stdout);
	while (true) {
		char c = '\0';
		read(STDIN_FILENO, &c, 1);
		if (c == '\177') {
			if (dp > 0) {
				//backspace
				memmove(input + dp - 1, input + dp, len - dp);
				input[len - 1] = '\0';
				i--;
				dp--;
				len--;
			}
		} else if (c == '\033') {
			//escape char
			getchar();
			c = getchar();
			if (c == 'A')
				//up arrow
				;
			else if (c == 'B')
				//down arrow
				;
			else if (c == 'C') {
				//right arrow
				if (dp < i) {
					i++;
					dp++;
				}
			} else if (c == 'D') {
				//left arrow
				if (dp > 0) {
					i--;
					dp--;
				}
			}
		} else if (c == ENDLINE) {
			//endline char
			input[i++] = '\n';
			printf("\n");
			break;
		} else {
			//normal char
			if (dp < i) {
				memmove(input + dp + 1, input + dp, len - dp);
			} else 
				input[i++] = c;
			dp++;
			len++;
		}
		printf("\r%s%s", prompt, input);
		for (int j = 0; j < len - dp; j++)
			printf("\b");
		fflush(stdout);
		if (i < 0)
			i = 0;
		if (dp < 0)
			dp = 0;
		if (len < 0)
			len = 0;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return input;
}
