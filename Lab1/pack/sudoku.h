
#ifndef SUDOKU_H
#define SUDOKU_H

#include<map>
using namespace std;
const bool DEBUG_MODE = false;
//NEIGHBOR: for a certain digit, it needs to match with 20 ambient digits which are at the colomn(6), row(6) and board(8).
enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
const int NUM = 9;

struct Ques{
	int serial;
	char puz[128];
};
struct Ans{
	int serial;
	int answer[N];
};
extern map<int, Ans>results;


#endif
