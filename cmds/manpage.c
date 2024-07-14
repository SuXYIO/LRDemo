#include "../main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

//print version
int printversion(int const argc, char** const argv)
{
	printf("Name: LR\nVersion: %s\n", VER);
	return 0;
}
//show help (manual) page
int manpage(int const argc, char** const argv)
{
	bool useraw = false;
	//tmp var
	int o = '?';
	optreset = true;
	if (argc > 1 && argv[1][0] != '-')
		optind = 2;
	else if (argc > 1 && argv[1][0] == '-')
		optind = 1;
	else
		optind = 1;
	while ((o = getopt(argc, argv, "r")) != -1) {
		switch (o) {
			case 'r':
				useraw = true;
				break;
			case '?':
				printf("%sFAIL: invalid option: '-%c'. %s\nUse \"h h\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sFAIL: unknown option: '-%c'. %s\nUse \"h h\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
				return -1;
		}
	}
	char filename[STR_BUFSIZE] = "\0";
	if (argc <= 1 || isemptystr(argv[1]) || argv[1][0] == '-') {
		strcpy(filename, "LR");
	} else {
		strcpy(filename, argv[1]);
	}
	char* const manual_path = "./manual/";
	//use txt or lnk
	char path[STR_BUFSIZE] = "\0";
	sprintf(path, "%s%s.txt", manual_path, filename);
	if (isfileexist(path) == false) {
		//try lnk
		sprintf(path, "%s%s.lnk", manual_path, filename);
		if (isfileexist(path) == false) {
			printf("%sFAIL: fail opening manual. Fopen fail. \n%s", COLOR_FAIL, COLOR_END);
			return -1;
		}
	}
	//use less or printf
	if (useraw == true || system("less 2> /dev/null") != 0) {
		//less unusable, use print
		FILE* file = NULL;
		char ch;
		file = fopen(path, "r");
		while ((ch = fgetc(file)) != EOF) {
			printf("%c", ch);
		}
		fclose(file);
		return 0;
	} else {
		char cmd[STR_BUFSIZE] = "\0";
		sprintf(cmd, "less -R %s", path);
		if (system(cmd) != 0) {
			printf("%sFAIL: fail opening manual. Less command fail. \n%s", COLOR_FAIL, COLOR_END);
			return -1;
		}
	}
	return 0;
}
