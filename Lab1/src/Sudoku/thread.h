#ifndef THREAD_H
#define THREAD_H


#include<iostream>
using namespace std;
#include <stdio.h>
#include<stdlib.h>
#include"sudoku.h"
#include<deque>
#include<string.h>
#include <assert.h>
deque<Datas*>list;
Datas *_d;
int total_solved = 0;
int total = 0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataready;

void print(Datas* d_ele){
	cout<<"nspaces=="<<d_ele->nspaces<<'\n';
	cout<<"board is\n";
	 for (int cell = 0; cell < N; ++cell) {
    	cout<<d_ele->board[cell];
 	 }
 	 cout<<'\n';
}
void* reader(void* temp){
	char puzzle[128];
	FILE* fp=(FILE*)temp;
	while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
	if (strlen(puzzle) >= N) {
		  pthread_mutex_lock(&mutex);
		  _d=new Datas();
		  ++total;
		  input(_d, puzzle);
		  list.push_back(_d);
		  //cout<<"i just pushed back:\n";
		 // print(_d);
		  pthread_mutex_unlock(&mutex);
		  pthread_cond_broadcast(&dataready);
		  //cout<<"broadcast  "<<total<<'\n';
		}
	  }
	cout<<"condition destroyed\n";
	pthread_cond_destroy(&dataready);
}

 bool (*solve)(Datas*, int) = solve_sudoku_dancing_links;

void* solver(void*){  
	int total1=-1;
	while(total1!=total){
			total1=total;
			pthread_mutex_lock(&mutex);
			while(list.empty()){
				cout<<pthread_self()<<"--solver awaits\n";
				pthread_cond_wait(&dataready, &mutex);
			}
				cout<<pthread_self()<<"--solver working\n";
				Datas *d_ele=list.front();
//				cout<<"in solver\n";
//				print(d_ele);
				list.pop_front();

				 init_cache( d_ele);
				
				 if (solve( d_ele, 0)) { 
					++total_solved;
					
				  if (!solved(d_ele)){
				 
				  	assert(0);
				  }
				 for(int i=0;i<N;++i){
		        	 cout<< d_ele->board[i];
		          }
		         cout<<'\n';
				 }
				  else {
					printf("No: ===");
				  }
				  pthread_mutex_unlock(&mutex);
	}
}

#endif







