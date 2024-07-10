#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//neuron f()
neuron nf;
//const neuron g()
neuron ng;
//extern options
short a_func_num = 0;
short l_func_num = 0;
int seed = 0;
int ret = 0;

char line[STR_BUFSIZE] = "\0";
int optn = 0;
char* opts[MAXARGS] = {"\0"};
int main(int const argc, char* const argv[])
{
	seed = strand();
	bool quit = false;
	for (int i = 0; i < MAXARGS; i++)
		opts[i] = malloc(STR_BUFSIZE * sizeof(char));
	do {
		char* linep = line;
		char prompt[STR_BUFSIZE] = "\0";
		//return value to prompt color
		if (ret == 0)
			sprintf(prompt, "%s%s%s", COLOR_NORM, PROMPT, COLOR_END);
		else
			sprintf(prompt, "%s%s%s", COLOR_FAIL, PROMPT, COLOR_END);
		//get line
		linep = inputline(prompt);
		strcpy(line, linep);
		linep = line;
		//line to args
		toargs();
		//execute command
		if (msc(opts[0], 3, "h", "help", "man") == true)
			ret = manpage(optn, opts);
		else if (msc(opts[0], 3, "v", "ver", "ver") == true)
			ret = printversion();
		else if (msc(opts[0], 3, "q", "quit", "exit") == true)
			quit = true;
		else if (line[0] == '!') {
			char cmd[STR_BUFSIZE] = "\0";
			strcpy(cmd, line + 1);
			for (int i = 1; i < optn; i++)
				sprintf(cmd, "%s %s", cmd, opts[i]);
			ret = system(cmd);
		}
		else if (msc(opts[0], 2, "i", "init") == true)
			ret = init(optn, opts);
		else if (msc(opts[0], 2, "p", "print") == true)
			ret = print(optn, opts);
		else if (msc(opts[0], 2, "pn", "printn") == true)
			ret = printn(optn, opts);
		else if (msc(opts[0], 2, "sr", "seedrand") == true)
			ret = seedrand(optn, opts);
		else if (msc(opts[0], 2, "tr", "train") == true)
			ret = train(optn, opts);
		else {
			printf("%sFAIL: unknown command: \"%s\". \n%s", COLOR_FAIL, opts[0], COLOR_END);
			ret = -1;
		}
		optn = 0;
		for (int i = 0; i < MAXARGS; i++)
			if (strcmp(opts[i], "\0") != 0)
				for (int j = 0; j < STR_BUFSIZE; j++)
					opts[i][j] = '\0';
	} while (quit == false);
	return 0;
}
