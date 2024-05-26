all: sudoku.exe

sudoku.exe: sudoku.c function.c function.h
	$(CC) -Wall -Wextra -ffast-math -O2 -o sudoku.exe sudoku.c function.c -lm
