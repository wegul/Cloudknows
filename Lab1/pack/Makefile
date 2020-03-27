CXXFLAGS+=-O2 -ggdb -DDEBUG
CXXFLAGS+=-Wall -Wextra

all: sudoku

sudoku: main.cc  dance.h sudoku.h
	g++ -pthread -g -O2 -o $@ $^
