// I, Pooja Mathur (730486257), pledge that I have neither given nor received unauthorized aid on this assignment.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tetris.h"

#define MAX_NAME_LENGTH 256
#define MAX_FILES 1000000

typedef struct {
	char *name;
	unsigned score;
	unsigned lines;
	float occupancy;
} Quicksave;

int compare_score(const void *a, const void *b) {
	Quicksave *qa = (Quicksave *) a;
	Quicksave *qb = (Quicksave *) b;
	if(qa->score != qb->score) {
		return qb->score - qa->score;
	} else if (qa->lines != qb->lines) {
		return qb->lines - qa->lines;
	} else if (qa->occupancy != qb->occupancy) {
		return qb->occupancy - qa->occupancy;
	}
}

int compare_lines(const void *a, const void *b) {
	Quicksave *qa = (Quicksave *) a;
	Quicksave *qb = (Quicksave *) b;
	if(qa->lines != qb->lines) {
		return qb->lines - qa->lines;
	} else if (qa->score != qb->score) {
		return qb->score - qa->score;
	} else if (qa->occupancy != qb->occupancy) {
		return qb->occupancy - qa->occupancy;
	}
}

int compare_occupancy (const void *a, const void *b) {
	Quicksave *qa = (Quicksave *) a;
	Quicksave *qb = (Quicksave *) b;
	if(qa->occupancy != qb->occupancy) {
		return qb->occupancy - qa->occupancy;
	} else if (qa->score != qb->score) {
		return qb->score - qa-> score;
	} else if (qa->lines != qb->lines) {
		return qb->lines - qa->lines;
	}
}

int calculate_occupancy(char *board) {
	int occupancy = 0;
	for(int x = 0; x < (BLOCKS_WIDE * BLOCKS_TALL); x++) {
			if(board[x] != 0) {
				occupancy++;
			}
	}
	return occupancy;
}

int main(int argc, char *argv[]) {
	if(argc < 3) {
		fprintf(stderr, "not enough arguments");
		return EXIT_FAILURE;
	}

	char *metric = argv[1];
	int n = atoi(argv[2]);

	if (n < 1 || n > MAX_FILES) {
		fprintf(stderr, "n must be positive and less than 1 million");
		return EXIT_FAILURE;
	}

	Quicksave* quicksaves = malloc(MAX_FILES * sizeof(Quicksave));
	int num_quicksaves = 0;

	char buffer[MAX_NAME_LENGTH];
	while(fgets(buffer, MAX_NAME_LENGTH, stdin) != NULL) {
		if(buffer[strlen(buffer)-1] == '\n') {
			buffer[strlen(buffer)-1] = '\0';
		}

		if(num_quicksaves >= MAX_FILES) {
			fprintf(stderr, "max quicksaves reached");
			return EXIT_FAILURE;
		}

		quicksaves[num_quicksaves].name = strdup(buffer);
		
		FILE* fp = fopen(buffer, "rb");
		if(fp == NULL) {
			fprintf(stderr, "failed to open file");
			continue;
		}
		
		TetrisGameState *state;
		state = malloc(sizeof(TetrisGameState));
		size_t sz = sizeof(TetrisGameState);
		if(fread(state, sz, 1, fp) != 1) {
			fprintf(stderr, "fai:wqled to read quicksave data");
			fclose(fp);
			return EXIT_FAILURE;
		}
		fclose(fp);
		quicksaves[num_quicksaves].score = state->score;
		quicksaves[num_quicksaves].lines = state->lines;
		quicksaves[num_quicksaves].occupancy = calculate_occupancy(state->board);

		num_quicksaves++;
		free(state);
	}

	if(!(strcmp(metric, "score"))) {
		qsort(quicksaves, num_quicksaves, sizeof(Quicksave), compare_score);
	} else if (!(strcmp(metric, "lines"))) {
		qsort(quicksaves, num_quicksaves, sizeof(Quicksave), compare_lines);
	} else if (!(strcmp(metric, "occupancy"))) {
		qsort(quicksaves, num_quicksaves, sizeof(Quicksave), compare_occupancy);
	} else {
		fprintf(stderr, "invalid metric, must be 'score', 'lines', or 'occupancy'.");
		return EXIT_FAILURE;
	}
	
	for (int i = 0; i < num_quicksaves && i < n; i++) {
		printf("%s\n", quicksaves[i].name);
	}

	free(quicksaves);
	return 0;
}
