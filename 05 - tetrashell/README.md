Tetrashell >:D

Tetrashell is an all-in-one tool to "hack" quicksaves to the top of the leaderboard.

## Building

download the Makefile, tetris.h and tetrashell.c (provided).

call ```make``` to download binaries and build the ```tetrashell``` executable to your current directory.

### Running
```bash
./tetrashell
```


## Features
- IAS: implemented as suggested in assignment writeup
### improved prompt
* improved prompt that displays username, hostname, and abbreviated name of current quicksave in color(s)!
### quick rank
* rank with 0 or 1 arguments, defaulting to 'score' and 10 players
### Commands
### modify
modifies the current quicksave\
```modify [score|lines] [value] // IAS```
### check
checks if the current quicksave passes legitimacy tests\
```check // IAS```
### info
provides the current quicksave's filepath, score, and lines\
```info // IAS```
### undo
reverts the changes from the last modify command\
```undo // reverts only the directly previous modify```
### recover
recovers quicksaves from a specified disk image file\
```recover [disk image filepath] // IAS```
### rank
uploads current quicksave to the ranking database, and prints the top players from the leaderboard\
```rank [score|lines (optional, def. score)] [number_of_players (optional, def. 10)] // IAS```
### switch
switches current quicksave to another specified quicksave\
```switch [new_quicksave_path] // IAS```
### help
prints the list of available commands if no arguments, or provides description and usage details for a specified command\
```help [command (slightly optional)] // IAS```
