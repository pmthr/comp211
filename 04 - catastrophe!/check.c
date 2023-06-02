// I, Pooja Mathur (730486257), pledge that I have neither given nor received unauthorized aid on this assignment.

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include "tetris.h"

// declare is_legitimate for link to verify.o
void is_legitimate(TetrisGameState* game, int* is_legit);

// declare buffer for segfault jump
sigjmp_buf jump_buffer;

// segfault handler
void handle_sigsegv(int sig) {
	siglongjmp(jump_buffer, 1);
}

int main(int argc, char* argv[]) {
	// verify # of arguments
	if(argc != 2) {
		fprintf(stderr, "invalid number of arguments");
		return 1;
	}

	// load quicksave into TGS
	FILE *fp = fopen(argv[1], "rb");
	if(fp == NULL) {
		fprintf(stderr, "could not open quicksave file %s\n", argv[1]);
		return 1;
	}
	TetrisGameState game;
	fread(&game, sizeof(game), 1, fp);
	fclose(fp);

	int is_legit = 0;
	
	// set appropriate jump buffer and associate SIGSEGV with its handler
	if(sigsetjmp(jump_buffer, 1) == 0) {
		signal(SIGSEGV, handle_sigsegv);
		// check legitimacy
		is_legitimate(&game, &is_legit); 
	}

	// print check result 
	printf(is_legit ? "is legitimate\n" : "is not legitimate\n");
	return 0;
}
