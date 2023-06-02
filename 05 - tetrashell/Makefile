EXECUTABLE = tetrashell

BINARIES_DIR = /playpen/a5

all: $(EXECUTABLE) copy_binaries

$(EXECUTABLE): tetrashell.c
	@gcc -o $(EXECUTABLE) tetrashell.c

copy_binaries:
		@cp -n $(BINARIES_DIR)/check .
		@cp -n $(BINARIES_DIR)/recover .
		@cp -n $(BINARIES_DIR)/rank .
		@cp -n $(BINARIES_DIR)/modify .
		@cp -n /home/pmathur/tetris.h .
clean:
	rm -f $(EXECUTABLE) $(BINARIES_DIR)/check $(BINARIES_DIR)/recover $(BINARIES_DIR)/rank $(BINARIES_DIR)/modify
