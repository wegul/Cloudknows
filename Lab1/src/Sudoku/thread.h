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
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataready;

void* reader(void* temp){
	FILE* fp=(FILE*)temp;
	char puzzle[128];
	while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
	if (strlen(puzzle) >= N) {
		  pthread_mutex_lock(&mutex);
		  ++total;
		  list.push_back(puzzle);	
		  pthread_mutex_unlock(&mutex);cout<<"reader working==="<<puzzle<<'\n';
		  //pthread_cond_broadcast(&dataready);
		  cout<<"broadcast\n";
		}
	  }
//	cout<<"condition destroyed\n";
//	pthread_cond_destroy(&dataready);
}

 bool (*solve)(int) = solve_sudoku_dancing_links;
void* solver(void*){ 
	while(1){
	cout<<"list_size is  "<<list.size()<<'\n';
	pthread_mutex_lock(&mutex);
		while(list.empty()){
			cout<<pthread_self()<<"--solver awaits\n";
			//pthread_cond_wait(&dataready, &mutex);
		}
			cout<<pthread_self()<<"--solver working\n";
			char *puzzle=list.front();
			//cout<<"current puzzle is "<<list.front()<<'\n';
			list.pop_front();
			input(puzzle);
			 init_cache();
			 if (solve(0)) {
				++total_solved;
			  if (!solved())
				assert(0);
		     for(int i=0;i<N;++i){
            	 cout<<board[i];
              }
             cout<<'\n';
			 }
			  else {
				printf("No: %s", puzzle);
			  }
			  pthread_mutex_unlock(&mutex);
	}
}

#endif







