// I, Pooja Mathur (730486257), pledge that I have neither given nor received unauthorized aid on this assignment.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "tetris.h"


int main(int argc, char *argv[]) {
	
	TetrisGameState *tgame;
	size_t sz = sizeof(TetrisGameState);
	tgame = malloc(sz);
	
	FILE *fp;
	if(argc!= 3) {
		fprintf(stderr, "%s", "Error: invalid number of arguments!");
		return EXIT_FAILURE;
	}

	else {
		fp = fopen("tetris_quicksave.bin","rb+");
		fread(tgame,sz,1, fp);
		fseek(fp,0,SEEK_SET);
		
		long to_mod = atol(argv[2]);
		if(to_mod < 0 ||to_mod > UINT_MAX) {
			fclose(fp);
			free(tgame);
			fprintf(stderr,"%s", "Error: invalid modifying value! Must be between 0 and uint_max.");
			return EXIT_FAILURE;
		}
		else {
			if(!strcmp(argv[1], "score")) {
				tgame->score = (unsigned int) to_mod;
			}	
			if(!strcmp(argv[1], "lines")) {
				tgame->lines = (unsigned int) to_mod;
			}	
			if(!strcmp(argv[1], "next_piece")) {
				if(to_mod < 0 || to_mod >= 19) {
					fclose(fp);
					free(tgame);
					fprintf(stderr, "%s", "Error: invalid next piece! Must be between 0 and 18, there are 19 total pieces.");
					return EXIT_FAILURE;
				}
				else {
					tgame->next_piece = (short int) to_mod;
				}
			}
		}
	}

	fwrite(tgame, sz, 1, fp);
	fclose(fp);
	free(tgame);	
	return EXIT_SUCCESS;
}
