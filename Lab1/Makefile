CXXFLAGS+=-O2 -ggdb -DDEBUG
CXXFLAGS+=-Wall -Wextra

all: sudoku_solve

sudoku_solve: main.cc  dance.h sudoku.h
	g++ -pthread -g -O2 -o $@ $^
