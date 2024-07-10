#include "../main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

neuron nf;
neuron ng;
extern int seed;
extern int ret;

//print values
int print(int const argc, char** const argv)
{
	char fmtstr[STR_BUFSIZE] = "\0";
	//tmp var
	int o = '?';
	int tmp = 0;
	optreset = true;
	optind = 1;
	while ((o = getopt(argc, argv, "v:")) != -1) {
		switch (o) {
			case 'v':
				strcpy(fmtstr, optarg);
				break;
			case '?':
				printf("%sFAIL: invalid option: '-%c'. %s\nUse \"h p\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sFAIL: unknown option: '-%c'. %s\nUse \"h p\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
				return -1;
		}
	}
	tmp = strlen(fmtstr);
			for (int i = 0; i < tmp; i++) {
				if (fmtstr[i] == '%') {
					i++;
					if (fmtstr[i] == 's')
						printf("%d", seed);
					else if (fmtstr[i] == 'r')
						printf("%d", ret);
					else if (fmtstr[i] == '%')
						printf("%%");
					else {
						printf("%s\nFAIL: invalid option: '-%c %s'. %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
						return 1;
					}
				} else
					printf("%c", fmtstr[i]);
			}
	printf("\n");
	return 0;
}

//print values for neuron
//TODO: redesign to support mutineuron outputting
int printn(int const argc, char** const argv)
{
	char fmtstr[STR_BUFSIZE] = "%w, %b, %l, %W, %B\n";
	bool tofile = false;
	bool p_f = true;
	bool p_g = true;
	FILE* fp = NULL;
	//local floating point precision
	int lfpp = FPP;
	//tmp var
	int o = '?';
	int tmp = 0;
	optreset = true;
	optind = 1;
	while ((o = getopt(argc, argv, "n:f:F:p:")) != -1) {
		switch (o) {
			case 'n':
				if (strcmp(optarg, "f") == 0) {
					p_f = true;
					p_g = false;
				} else if (strcmp(optarg, "g") == 0) {
					p_f = false;
					p_g = true;
				} else if (strcmp(optarg, "a") == 0) {
					p_f = true;
					p_g = true;
				} else {
					printf("%sFAIL: invalid option: '-%c %s'. %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
					return 1;
				}
				break;
			case 'f':
				strcpy(fmtstr, optarg);
				break;
			case 'F':
				tofile = true;
				short t = isusablefile(optarg);
				if (t == 0)
					fp = fopen(optarg, "w");
				else if (t == 1)
					tofile = false;
				else if (t == 2)
					return 1;
				else {
					printf("%sFAIL: isusablefile() fail. \n%s", COLOR_FAIL, COLOR_END);
					return 1;
				}
				break;
			case 'p':
				lfpp = atoi(optarg);
				break;
			case '?':
				printf("%sFAIL: invalid option: '-%c'. %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, optopt, COLOR_NORM, COLOR_END);
				return -1;
				break;
			default:
				printf("%sFAIL: unknown option: '-%c'. %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, o, COLOR_NORM, COLOR_END);
				return -1;
		}
	}
	short dotime = 1;
	tmp = strlen(fmtstr);
	neuron* npt = NULL;
	if (p_f == true)
		npt = &nf;
	if (p_g == true)
		npt = &ng;
	if (p_f == true && p_g == true)
		dotime = 2;
	for (int i = 0; i < dotime; i++) {
		if (dotime == 2) {
			if (i == 0)
				npt = &nf;
			else if (i == 1)
				npt = &ng;
		}
		if (tofile == false) {
			for (int i = 0; i < tmp; i++) {
				if (fmtstr[i] == '%') {
					i++;
					if (fmtstr[i] == 'w')
						printf("%.*f", lfpp, (*npt).w);
					else if (fmtstr[i] == 'b')
						printf("%.*f", lfpp, (*npt).b);
					else if (fmtstr[i] == 'l')
						printf("%.*f", lfpp, (*npt).l);
					else if (fmtstr[i] == 'W')
						printf("%.*f", lfpp, (*npt).wg);
					else if (fmtstr[i] == 'B')
						printf("%.*f", lfpp, (*npt).bg);
					else if (fmtstr[i] == '%')
						printf("%%");
					else {
						printf("%s\nFAIL: invalid option: '-%c %s'. %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
						return 1;
					}
				} else
					printf("%c", fmtstr[i]);
			}
		} else if (tofile == true) {
			for (int i = 0; i < tmp; i++) {
				if (fmtstr[i] == '%') {
					i++;
					if (fmtstr[i] == 'w')
						fprintf(fp, "%.*f", lfpp, (*npt).w);
					else if (fmtstr[i] == 'b')
						fprintf(fp, "%.*f", lfpp, (*npt).b);
					else if (fmtstr[i] == 'l')
						fprintf(fp, "%.*f", lfpp, (*npt).l);
					else if (fmtstr[i] == 'W')
						fprintf(fp, "%.*f", lfpp, (*npt).wg);
					else if (fmtstr[i] == 'B')
						fprintf(fp, "%.*f", lfpp, (*npt).bg);
					else if (fmtstr[i] == '%')
						fprintf(fp, "%%");
					else {
						printf("%s\nFAIL: invalid option: '-%c %s' (file written). %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, o, optarg, COLOR_NORM, COLOR_END);
						return 1;
					}
				} else
					fprintf(fp, "%c", fmtstr[i]);
			}
		}
	}
	printf("\n");
	return 0;
}
