#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//neuron f()
neuron nf;
//training neuron g()
neuron ng;
//extern options
short a_func_num = 0;
short l_func_num = 0;
int seed = 0;

int main(int const argc, char* const argv[])
{
	seed = strand();
	bool quit = false;
	int ret = 0;
	do {
		char line[STR_BUFSIZE] = "\0";
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
		int optn = 0;
		char* opts[MAXARGS] = {NULL};
		//tokenize
		char* tmp = NULL;
		do {
			opts[optn++] = tmp = strsep(&linep, CMDSEP);
		} while (tmp != NULL && optn < MAXARGS);
		optn--;
		//execute command
		if (msc(opts[0], 3, "h", "help", "man") == true)
			ret = manpage(opts[0]);
		else if (msc(opts[0], 3, "v", "ver", "ver") == true)
			ret = printversion();
		else if (msc(opts[0], 3, "q", "quit", "exit") == true)
			quit = true;
		else if (line[0] == '!')
			ret = system(line + 1);
		else if (msc(opts[0], 2, "i", "init") == true)
			ret = init(optn, opts);
		else if (msc(opts[0], 2, "sr", "seedrand") == true)
			ret = seedrand(optn, opts);
		else if (msc(opts[0], 2, "tr", "train") == true)
			ret = train(optn, opts);
		else {
			printf("%sFAIL: unknown command: \"%s\". \n%s", COLOR_FAIL, opts[0], COLOR_END);
			ret = -1;
		}
	} while (quit == false);
	return 0;
}
