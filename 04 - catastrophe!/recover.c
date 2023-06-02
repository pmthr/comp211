// I, Pooja Mathur (730486257), pledge that I have neither given nor received unauthorized aid on this assignment.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/stat.h>
#include <errno.h>
#include "tetris.h"

#define BLOCK_SIZE 4096
#define QUICKSAVE_SIZE sizeof(TetrisGameState)

// declare for link to verify.o
void is_legitimate(TetrisGameState* game, int* is_legit);

// copied over directly from tetris.c
static int SanityCheckState(TetrisGameState *s) {
	int row, col, tmp;
	char c;
	tmp = s->location_x;
	if ((tmp < 0) || (tmp >= BLOCKS_WIDE)) return 0;
	tmp = s->location_y;
	if ((tmp < PIECE_START_Y) || (tmp >= BLOCKS_TALL)) return 0;
	tmp = sizeof(tetris_pieces) / sizeof(const char*);
	if ((s->current_piece < 0) || (s->current_piece >= tmp)) return 0;
	if ((s->next_piece < 0) || (s->next_piece >= tmp)) return 0;
	for (row = 0; row < BLOCKS_TALL; row++) {
		for (col = 0; col < BLOCKS_WIDE; col++) {
			c = s->board[row * BLOCKS_WIDE + col];
			if ((c < ' ') || (c > '~')) return 0;
		}
	}

	return 1;
}

// declare segfault jump buffer;
sigjmp_buf jump_buffer;

// same segfault handler as check.c
void handle_sigsegv(int sig) {
	siglongjmp(jump_buffer, 1);
}

// function for saving file to directory
int save_quicksave(TetrisGameState *state, int filenum) {
	char dir[] = "./output";
	char filename[256];
	FILE * f;

	if(filenum == 1) {
		// make directory when saving file #1
		if(mkdir(dir, 0777) != 0 && errno != EEXIST) {
			fprintf(stderr,"can't make directory");
			perror(NULL);
			return 1;
		}

	}
	
	// generate file name based on file #
	snprintf(filename, sizeof(filename), "%s/recovered_%d.bin", dir, filenum);
	
	// write quicksave to file
	f = fopen(filename, "wb");
	if(!f) {
		fprintf(stderr, "could not open file for writing");
		return 1;
	}

	fwrite(state, 1, QUICKSAVE_SIZE, f);
	fclose(f);
	printf("%s\n", filename);
	return 0;
}

int main(int argc, char* argv[]) {
	// argument check
	if(argc != 2) {
		fprintf(stderr, "invalid # of arguments");
		return 1;
	}
	
	char* disk_path = argv[1];
	FILE* fp = fopen(disk_path, "rb");
	if (fp == NULL) {
		fprintf(stderr, "unable to open disk image file");
		return 1;
	}
	
	int blocknum = 0;
	int filenum = 1;
	size_t bytes;
	char block[BLOCK_SIZE];
	TetrisGameState state;
	// loop through blocks, read quicksave data for each
	while((bytes = fread(&state, 1, QUICKSAVE_SIZE, fp)) == QUICKSAVE_SIZE) {
		int is_legit = 0;
		// set segfault jump, same as check.c
		if(sigsetjmp(jump_buffer, 1) == 0) {
			signal(SIGSEGV, handle_sigsegv);
			is_legitimate(&state, &is_legit);
		}

		// check if quicksave is both valid and legitimate
		if (SanityCheckState(&state) && is_legit == 1) {
			save_quicksave(&state, filenum);
			filenum++;
		}

		blocknum++;

		// move to beginning of next block
		fseek(fp, blocknum * BLOCK_SIZE, SEEK_SET);
	}

	fclose(fp);
	return 0;	
}
