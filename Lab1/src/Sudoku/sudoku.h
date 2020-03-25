#ifndef SUDOKU_H
#define SUDOKU_H


const bool DEBUG_MODE = false;
//NEIGHBOR: for a certain digit, it needs to match with 20 ambient digits which are at the colomn(6), row(6) and board(8).
enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
const int NUM = 9;

//extern int neighbors[N][NEIGHBOR];
//extern int board[N];
//extern int spaces[N];
//extern int nspaces;
//extern int (*chess)[COL];
struct Datas{//cannot be modified by key_word EXTERN
	int serial;
	int neighbors[N][NEIGHBOR];
	int board[N];
	int (*chess)[COL];
};
struct Ans{
	int q[N];
	int a[N];
};
void print(Datas* d_ele);
void init_neighbors();
void input(Datas* d_ele, const char in[N]);


bool available(Datas* d_ele, int guess, int cell);


bool solve_sudoku_dancing_links(Datas* d_ele, int unused);
bool solved(Datas* d_ele);
#endif
