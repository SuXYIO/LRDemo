#include "../main.h"

extern bool debugmode;

int enabledebug(int const argc, char** const argv)
{
	debugmode = true;
	return 0;
}
