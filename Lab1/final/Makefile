CXXFLAGS+=-O2 -ggdb -DDEBUG
CXXFLAGS+=-Wall -Wextra

all: sudoku

sudoku: main.cc neighbor.cc  sudoku_dancing_links.cc thread.h
	g++ -pthread -O2 -o  $@ $^ 
