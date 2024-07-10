#include "../main.h"
#include <stdio.h>

int printstr(int const argc, char** const argv)
{
	for (int i = 1; i < argc; i++)
		printf("%s\n", argv[i]);
	return 0;
}
