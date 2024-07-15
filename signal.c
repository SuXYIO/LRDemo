#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sighandler(int sig) {
	printf("%s\nSignal %d received, terminating...\n%s", COLOR_WARN, sig, COLOR_END);
	exit(sig);
}
