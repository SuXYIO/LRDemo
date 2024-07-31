#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

void sighandler(int sig) {
	printf("%s\nSignal %d received\n%s", COLOR_WARN, sig, COLOR_END);
	printf("%sQuit[y/n]> %s", COLOR_WARN, COLOR_END);
	fflush(stdout);
	char c = '\0';
	read(STDIN_FILENO, &c, 1);
	printf("%c\n", c);
	fflush(stdout);
	if (c == 'y')
		exit(sig);
	else
		return;
}
