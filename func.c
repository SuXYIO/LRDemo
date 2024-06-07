#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//get weights & biases
	extern double f_w;
	extern double f_b;
	extern double g_w;
	extern double g_b;

//calculate batch thread function
void* calc_batch(void* args) {
	//get args
	void** arg = (void**) args;
	double* pl = arg[0];
	double* pgrad_w = arg[1];
	double* pgrad_b = arg[2];
	//calc
	double x = rand_nml(1.0, 1.0);
	double y_f = f(x);
	double y_g = g(x);
	//pass results
	*pl += MSE(y_g, y_f);
	*pgrad_w += MSE_grad_w(y_g, x);
	*pgrad_b += MSE_grad_b(y_g, x);
	return NULL;
}

//other function stuff
//test file for writing
int isusablefile(char* filename) {
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
int printversion(void) {
	printf("Name: LR\nVersion: %s\n", VER);
	return 0;
}
//show help (manual) page
int manualpage(void) {
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
