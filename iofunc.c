#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <termios.h>

#define BACKSPACE '\177'
#define ESCSEQCHAR '\033'
#define BELLCHAR '\a'
//input a line
char* inputline(char* prompt)
{
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	//init
	static char input[STR_BUFSIZE] = "\0";
	static char cmdhist[STR_BUFSIZE] = "\0";
	for (int i = 0; i < STR_BUFSIZE; i++)
		input[i] = '\0';
	//distance to prompt
	int dp = 0;
	int len = 0;
	//hist stuff
	bool canhist = true;
	bool ishist = false;
	printf("%s", prompt);
	fflush(stdout);
	while (true) {
		char c = '\0';
		read(STDIN_FILENO, &c, 1);
		if (c == BACKSPACE) {
			if (dp > 0) {
				//backspace
				memmove(input + dp - 1, input + dp, len - dp);
				input[--len] = '\0';
				dp--;
			}
		} else if (c == ESCSEQCHAR) {
			//escape char
			getchar();
			c = getchar();
			if (c == 'A') {
				//up arrow
				if (canhist == true && ishist == false) {
					strcpy(input, cmdhist);
					ishist = true;
					dp = len = strlen(input);
				}
			} else if (c == 'B') {
				//down arrow
				if (canhist == true && ishist == true) {
					for (int i = 0; i < STR_BUFSIZE; i++)
						input[i] = '\0';
					ishist = false;
					dp = len = 0;
				}
			} else if (c == 'C') {
				//right arrow
				if (dp < len) {
					dp++;
					canhist = false;
				}
			} else if (c == 'D') {
				//left arrow
				if (dp > 0) {
					dp--;
					canhist = true;
				}
			}
		} else if (c == ENDLINE) {
			//endline char
			printf("\n");
			break;
		} else {
			//normal char
			if (len < STR_BUFSIZE - 1) {
				if (dp < len) {
					memmove(input + dp + 1, input + dp, len - dp);
					input[dp] = c;
				} else 
					input[dp] = c;
				dp++;
				len++;
				canhist = false;
			} else {
				//buffer full
				printf("%c", BELLCHAR);
				fflush(stdout);
			}
		}
		if (dp < 0)
			dp = 0;
		if (len < 0)
			len = 0;
		if (len == 0)
			canhist = true;
		printf("\033[2K\r%s%s", prompt, input);
		for (int j = 0; j < len - dp; j++)
			printf("\b");
		fflush(stdout);
	}
	if (isemptystr(input) == false)
		strcpy(cmdhist, input);
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return input;
}
