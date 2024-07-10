#include "main.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

//other function stuff
//test file for writing
bool isusablefile(char* const filename)
{
	/*
	ret: 
		0: enable overwrite
		1: forget write to file
		2: quit
		-1: error
	*/
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
bool isemptystr(char* const str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
		if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t' && str[i] != '\0')
			return false;
	return true;
}
//mutiple string cmpare
bool msc(char* str, int strsc, ...)
{
	bool same = false;
	va_list strs;
	va_start(strs, strsc);
	for (int i = 0; i < strsc; i++)
		if (strcmp(str, va_arg(strs, char*)) == 0) {
			same = true;
			break;
		}
	va_end(strs);
	return same;
}
