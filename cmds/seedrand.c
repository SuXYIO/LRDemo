#include "../main.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

extern int seed;

//seed random
int seedrand(int const argc, char** const argv)
{
	//tmp var
	int o = '?';
	optreset = true;
	optind = 1;
	while ((o = getopt(argc, argv, "s:")) != -1) {
		switch (o) {
			case 's':
				if (optarg == NULL) {
					printf("%sFAIL: option '-%c' requires an argument. %s\nUse \"h sr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
					return -1;
				}
				srand(atoi(optarg));
				seed = atoi(optarg);
				return 0;
				break;
			case '?':
				printf("%sFAIL: invalid option: '-%c'. %s\nUse \"h sr\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sFAIL: unknown option: '-%c'. %s\nUse \"h sr\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
				return -1;
		}
	}
	seed = strand();
	return 0;
}
