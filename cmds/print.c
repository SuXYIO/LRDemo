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
	char fmtstr[STR_BUFSIZE] = "%fw, %fb, %fl, %fW, %fB\n%gw, %gb, %gl, %gW, %gB";
	bool pnewl = true;
	bool tofile = false;
	FILE* fp = NULL;
	//local floating point precision
	int lfpp = FPP;
	//tmp var
	int o = '?';
	int tmp = 0;
	optreset = true;
	optind = 1;
	while ((o = getopt(argc, argv, "f:F:Np:")) != -1) {
		switch (o) {
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
			case 'N':
				pnewl = false;
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
	tmp = strlen(fmtstr);
	neuron* np = NULL;
	for (int i = 0; i < tmp; i++) {
		np = NULL;
		if (fmtstr[i] == '%') {
			i++;
			if (fmtstr[i] == 'f') {
				i++;
				np = &nf;
			} else if (fmtstr[i] == 'g') {
				i++;
				np = &ng;
			} else if (fmtstr[i] == 's') {
				pfunc(tofile, fp, "%d", seed);
			} else if (fmtstr[i] == 'r') {
				pfunc(tofile, fp, "%d", ret);
			} else if (fmtstr[i] == '%') {
				pfunc(tofile, fp, "%%");
			}
			if (np != NULL) {
				if (fmtstr[i] == 'w')
					pfunc(tofile, fp, "%.*f", lfpp, (*np).w);
				else if (fmtstr[i] == 'b')
					pfunc(tofile, fp, "%.*f", lfpp, (*np).b);
				else if (fmtstr[i] == 'l')
					pfunc(tofile, fp, "%.*f", lfpp, (*np).l);
				else if (fmtstr[i] == 'W')
					pfunc(tofile, fp, "%.*f", lfpp, (*np).wg);
				else if (fmtstr[i] == 'B')
					pfunc(tofile, fp, "%.*f", lfpp, (*np).bg);
				else {
					printf("%s\nFAIL: invalid option. %s\nUse \"h pn\" for help. \n%s", COLOR_FAIL, COLOR_NORM, COLOR_END);
					return 1;
				}
			}
		} else
			pfunc(tofile, fp, "%c", fmtstr[i]);
	}
	if (pnewl == true)
		pfunc(tofile, fp, "\n");
	return 0;
}
