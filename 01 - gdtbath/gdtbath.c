// I, Pooja Mathur (730486257), pledge that I have neither given nor received unauthorized aid on this assignment.

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
	char *user = getlogin();
	char *env = getenv("TERM");
	if(env != NULL && strcmp(env, "xterm-256color") == 0) {
		printf("Hello! %s thinks it's a \x1b[38;5;51mGDTBATH!\n\x1b[m", user);
	}
	else {
		printf("Hello! %s thinks it's a GDTBATH!\n", user);
	};
	return 0;
}
