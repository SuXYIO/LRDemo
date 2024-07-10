#include "../main.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

neuron nf;
neuron ng;

//init neurons
int init(int const argc, char** const argv)
{
	bool i_f = true;
	bool i_g = true;
	double (*ifunc)(void) = rand_nmlstd;
	//tmp var
	int o = '?';
	int tmp = 1;
	optreset = true;
	optind = 1;
	while ((o = getopt(argc, argv, "n:m:")) != -1) {
		switch (o) {
			case 'n':
				if (strcmp(optarg, "a") == 0) {
					i_f = true;
					i_g = true;
				} else if (strcmp(optarg, "f") == 0)
					i_f = true;
				else if (strcmp(optarg, "g") == 0)
					i_g = true;
				else {
					printf("%sFAIL: invalid option: '-%c %s'. %s\nUse \"h i\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
					return -1;
				}
				break;
			case 'm':
				tmp = atoi(optarg);
				if (tmp == 0)
					ifunc = r0;
				else if (tmp == 1)
					ifunc = rand_nmlstd;
				else {
					printf("%sFAIL: invalid option: '-%c %s'. %s\nUse \"h i\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
					return -1;
				}
				break;
			case '?':
				printf("%sFAIL: invalid option: '-%c'. %s\nUse \"h i\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sFAIL: unknown option: '-%c'. %s\nUse \"h i\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
				return -1;
		}
	}
	if (i_f == true) {
		//neuron f
		nf.w = ifunc();
		nf.b = ifunc();
		nf.l = 0.0;
		nf.wg = 0.0;
		nf.bg = 0.0;
	}
	if (i_g == true) {
		//neuron g
		ng.w = ifunc();
		ng.b = ifunc();
	}
	return 0;
}
