#ifndef THREAD_H
#define THREAD_H


#include<iostream>
using namespace std;
#include <stdio.h>
#include"sudoku.h"
#include<deque>
#include<string.h>
#include <assert.h>
deque<char*>list;
int total_solved = 0;
int total = 0;


void* reader(void* temp){
	FILE* fp=(FILE*)temp;
	char puzzle[128];
	while (fgets(puzzle, 128, fp) != NULL) {
    if (strlen(puzzle) >= N) {
      ++total;
      list.push_back(puzzle);
      break;
    }
  }
  
}

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
 bool (*solve)(int) = solve_sudoku_dancing_links;
void* solver(void*){
	pthread_mutex_lock(&mutex);
	while(!list.empty()){
		char *puzzle=list.front();
		list.pop_front();
		input(puzzle);
		 init_cache();
		 if (solve(0)) {
		    ++total_solved;
		  if (!solved())
		    assert(0);
		 }
		  else {
		    printf("No: %s", puzzle);
		  }
	}
	pthread_mutex_unlock(&mutex);
}

#endif







