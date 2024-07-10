#include "../main.h"

extern bool debugmode;

int disabledebug(int const argc, char** const argv)
{
	debugmode = false;
	return 0;
}
