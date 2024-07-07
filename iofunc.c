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
//command hist
int hn = 0;
char cmdhist[CMDHISTSIZE][STR_BUFSIZE] = {"\0"};
//input a line
char* inputline(char* prompt)
{
	//TODO: Fix cmdhist segfault
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	//init
	static char input[STR_BUFSIZE] = "\0";
	//hist selection
	//0 if not selected
	int hs = 0;
	for (int i = 0; i < STR_BUFSIZE; i++)
		input[i] = '\0';
	//distance to prompt
	int dp = 0;
	int len = 0;
	//hist stuff
	bool canhist = true;
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
				if (canhist == true) {
					hs++;
					if (hs > hn)
						hs = hn;
					strcpy(input, cmdhist[hs - 1]);
					dp = len = strlen(input);
				}
			} else if (c == 'B') {
				//down arrow
				if (canhist == true && hs != 0) {
					hs--;
					if (hs < 0)
						hs = 0;
					if (hs != 0) {
						strcpy(input, cmdhist[hs - 1]);
						dp = len = strlen(cmdhist[hs - 1]);
					} else if (hs == 0) {
						strcpy(input, "\0");
						dp = len = 0;
					}
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
					canhist = false;
				}
			}
		} else if (c == ENDLINE) {
			//endline char
			printf("\n");
			input[len] = '\0';
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
		if (dp > len)
			dp = len;
		if (hs > hn)
			hs = hn;
		if (hs < 0)
			hs = 0;
		if (dp > 0)
			printf("\033[M\r%s%s\r%s\033[%iC", prompt, input, prompt, dp);
		else if (dp == 0)
			printf("\033[M\r%s%s\r%s", prompt, input, prompt);
		fflush(stdout);
	}
	if (isemptystr(input) == false) {
		for (int i = CMDHISTSIZE - 1; i > 0; i--) {
			if (cmdhist[i] != NULL)
				strcpy(cmdhist[i], cmdhist[i - 1]);
		}
		strcpy(cmdhist[0], input);
		hn++;
		if (hn > CMDHISTSIZE)
			hn = CMDHISTSIZE;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return input;
}
