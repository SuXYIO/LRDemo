#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//neuron f()
neuron nf;
//training neuron g()
neuron ng;
//extern options
int a_func_num = 0;
int l_func_num = 0;
int seed = 0;
double eta = 0.001;
double l_exp = 0.0001;
int batch_size = 256;
int thread_size = 0;
bool verbose = false;
bool use_thread = false;
bool writetofile = false;
char csvfilename[STR_BUFSIZE];
FILE* csvfilep = NULL;


int main(int const argc, char* const argv[])
{
	bool quit = false;
	do {
		char line[STR_BUFSIZE] = "\0";
		char* linep = line;
		//get line
		linep = inputline(PROMPT);
		strcpy(line, linep);
		char* tmp = NULL;
		char* cmd = NULL;
		char* opts[MAXARGS] = {NULL};
		//tokenize
		line[strcspn(line, "\n")] = '\0';
		int i = 0;
		cmd = strsep(&linep, CMDSEP);
		do
			opts[i++] = tmp = strsep(&linep, CMDSEP);
		while (tmp != NULL && i < MAXARGS);
		//execute command
		if (msc(cmd, 3, "h", "help", "man") == true)
			manpage(opts[0]);
		else if (msc(cmd, 3, "v", "ver", "ver"))
			printversion();
		else if (msc(cmd, 3, "q", "quit", "exit"))
			quit = true;
		else
			if (line[0] == '!') {
				system(line + 1);
			} else
				printf("%sFAIL: unknown command: \"%s\". \n%s", COLOR_FAIL, cmd, COLOR_END);
	} while (quit == false);
	return 0;
}
