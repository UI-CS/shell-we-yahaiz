CC = gcc
CFLAGS = -Wall -g -Iinclude

all: shell sudoku monte_carlo

shell: src/main.c src/builtins.c
	$(CC) $(CFLAGS) src/main.c src/builtins.c -o shell

sudoku: src/sudoku.c
	$(CC) $(CFLAGS) src/sudoku.c -o sudoku -pthread

monte_carlo: src/monte_carlo.c
	$(CC) $(CFLAGS) src/monte_carlo.c -o monte_carlo -pthread

clean:
	rm -f shell sudoku monte_carlo 