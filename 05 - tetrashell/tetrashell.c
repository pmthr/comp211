// I, Pooja Mathur (730486257), pledge that I have neither given nor received unauthorized aid on this assignment.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include "tetris.h"

// execution handler for most commands
void execute(char *command, char *args[]) {
	pid_t pid = fork();
	if(pid == 0) { // child process
		execv(command, args);
	} else if(pid > 0) { // parent process
		int status;
		wait(&status);
	} else {
		printf("Failed to fork()\n");
		exit(1);
	}
}

// switch handler
void execute_switch(char *input, char *filepath) {
	printf("Switched quicksave from %s to %s.\n", filepath, input);
	strcpy(filepath, input);
}

// info handler
void execute_info(char *filepath) {
	TetrisGameState game;
	// read quicksave into TetrisGameState to print info
	FILE *f = fopen(filepath, "rb");
	if(!f) {
		fprintf(stderr, "Could not open file %s\n", filepath);
		exit(1);
	}
	if(fread(&game, sizeof(TetrisGameState), 1, f) != 1) {
		fprintf(stderr, "Could not read file %s\n", filepath);
		exit(1);	
	}
	// print info
	printf("Current Savefile: %s\nScore: %u\nLines: %u\n",
			filepath, game.score, game.lines);
}

// rank handler
void execute_rank(char *filepath, char *field, char *value) {
	// create pipe
	int fd[2];
	if(pipe(fd) == -1) {
		fprintf(stderr, "Error with pipe\n");
		exit(1);
	}
	pid_t pid = fork();
	if(pid == 0) { // child process
		close(fd[1]); // close unused write end
		if(dup2(fd[0], STDIN_FILENO) == -1) {
			fprintf(stderr, "Error with dup2\n"); // duplicate
			exit(1);
		}
		close(fd[0]);
		char *args[] = {"rank", field, value, "uplink", NULL};
		execv("./rank", args);
	} else if(pid > 0) {
		close(fd[0]); // close unused read end
		if(write(fd[1], filepath, strlen(filepath) 
					!= strlen(filepath))) { 
					// write to write end
			fprintf(stderr, "Error writing to pipe\n.");
			exit(1);
		}
		close(fd[1]);  // close write end
		int status;
		wait(&status);
		char buf[256];
		int line = 1;
	} else {
		fprintf(stderr, "Failed to fork()\n");
		exit(1);
	}
}

// help handler
void execute_help(char *command) {
	if(strcmp(command, "modify") == 0) {
		printf("Modify the current quicksave by a certain value.\n");
		printf("Usage: modify [score|lines] [value] \n");
	} else if(strcmp(command, "rank") == 0) { 
		 printf("Add the current quicksave to the leaderboard and display the top players.\n");
		 printf("Usage: rank [metric (optional)] [num_top_games(optional]\n");
		 printf("By default, ranks by score and displays the top 10 players.\n");
	} else if(strcmp(command, "recover") == 0) {
		printf("Recover quicksaves from a disk image file.\n");
		printf("Usage: recover [disk_image_file]\n");
	} else if (strcmp(command, "check") == 0) {
		printf("Verify if the current quicksave passes legitimacy checks.\n");
		printf("Usage: check\n");
	} else if(strcmp(command, "undo") == 0) {
		printf("Undoes the last change to the current quicksave.\n");
		printf("Usage: undo\n");
	} else if(strcmp(command, "switch") == 0) {
		printf("Switch the current quicksave to a different one.\n");
		printf("Usage: switch [new_quicksave_path]\n");
	} else if(strcmp(command, "info") == 0) {
		printf("Display the filepath, score, and lines cleared of the current quicksave.\n");
		printf("Usage: info\n");
	} else if(strcmp(command, "exit") == 0) {
		printf("Exits out of tetrashell.\n");
		printf("Usage: exit\n");
	} else {
		printf("Unknown command. Type 'help' for a list of available commands.\n");
	}
}

// backup file for undo support
void backup_file(char *filepath, char *filepath2) {
	TetrisGameState game;
	// copy contents of quicksave into backup
	FILE *f = fopen(filepath, "rb");
	FILE *f2 = fopen(filepath2, "wb");
	if(!f || !f2) {
		fprintf(stderr, "Error opening file\n");
		exit(1);
	}
	if(fread(&game, sizeof(TetrisGameState), 1, f) != 1) {
		fprintf(stderr, "Error reading file\n");
		exit(1);
	}
	fclose(f);
	fwrite(&game, sizeof(TetrisGameState), 1, f2);
	fclose(f2);	
}

// main loop
int main() {
	char filepath[256];
	char input[256];
	printf("Welcome to Tetrashell!\n");
	int exists = 0;
	do { // prompt for file until an existing filepath is given
		printf("Enter the path of the quicksave you're hacking >:D\n");
		fgets(filepath, sizeof(filepath), stdin);
		filepath[strcspn(filepath, "\n")] = '\0'; // remove newline
		if(access(filepath, F_OK) == 0) {
			exists = 1;
		} else {
			printf("Quicksave %s does not exist. Try again.\n", 
					filepath);
		}
	} while(!exists);
	char *user = getlogin();
	char hostname[256];
	gethostname(hostname, sizeof(hostname));
	char abr[6]; // abbreviate to first 5 characters of filepath
	// enter main loop for commands
	while(1) {
		// improved prompt - 7 pts
		strncpy(abr, filepath, 5);
		printf(
		"\033[1;34m%s\033[0m@\033[1;36m%s\033[0m\033[1;35m[%s]\033[0m> "
			, user, hostname, abr);
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = '\0'; // remove newline
		// exit - 5 pts
		if(strcmp(input, "exit") == 0) {
			printf("Exiting tetrashell. Goodbye!\n");
			return 0;
		}
		// help - 7 pts
		else if(strncmp(input, "help", 4) == 0) {
			// 4 catches for list of commands
			char *command = strtok(input + 5, "");
			if(!command) {
				printf("Available commands:\n");
				printf("info, switch, modify, recover, check, undo, rank\n");
			} else {
			execute_help(command);
			}
		}
		// info - 5 pts
		else if(strcmp(input, "info") == 0) {
			execute_info(filepath);
		}
		// switch - 5 pts
		else if(strncmp(input, "switch ", 7) == 0) {
			execute_switch(input + 7, filepath);
		}
		// recover - 10 pts
		else if(strncmp(input, "recover ", 8) == 0) {
			char *args[] = {"recover", input + 8, NULL};
			execute("./recover", args);
		}
		// check - 15 pts
		else if(strcmp(input, "check") == 0) {
			char *args[] = {"check", filepath, NULL};
			// 'automatically' pass filepath
			execute("./check", args);
		}
		// modify - 15 pts
		else if(strncmp(input, "modify ", 7) == 0) {
			char *field = strtok(input + 7, " ");
			char *value = strtok(NULL, "");
			char *args[] = {"modify", field, value, filepath, 
				NULL};
			// 'automatically' pass filepath
			// make backup for undo support
			backup_file(filepath, "./backup.bin");
			execute("./modify", args);
		}
		// undo - 15 pts
		else if(strcmp(input, "undo") == 0) {
			// copy contents of backup back to file
			backup_file("./backup.bin", filepath);
			// need to modify again to undo again
			remove("./backup.bin");
			printf("Undid last modification.\n");
		}
		// rank - 25 pts
		else if(strncmp(input, "rank", 4) == 0) {
			// 4 catches rank with no params
			char *field = strtok(input + 5, " ");
			char *value = strtok(NULL, "");
			// quick rank - 5 pts
			if(!field) {
				field = "score"; // by default
			}
			if(!value) {
				value = "10"; // by default
			}
			execute_rank(filepath, field, value);
		}
		else {
			printf("Unknown command. Try again, or type 'help' for available commands.\n");
		}
	}	
	return 0;
}
