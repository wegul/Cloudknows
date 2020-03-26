#ifndef THREAD_H
#define THREAD_H

#include<iostream>
using namespace std;
#include <stdio.h>
#include<stdlib.h>
#include"sudoku.h"
#include<deque>
#include<map>
#include<string.h>
#include <assert.h>
deque<Datas*>list;
map<int, Ans*>results;

Datas *_d;
Ans* _a;

int total_solved = 0;
int total = 0;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER, mutex2=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataready;

void input(Datas* d_ele, const char in[N])
{
	d_ele->serial=total;
	d_ele->chess= (int (*)[COL])(d_ele ->board);
  for (int cell = 0; cell < N; ++cell) {
    d_ele->board[cell] = in[cell]- '0';
  }
}

void* reader(void* temp){
	char puzzle[128];
	FILE* fp=(FILE*)temp;
	while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
	if (strlen(puzzle) >= N) {
		  pthread_mutex_lock(&mutex1);
		  _d=new Datas();
		  ++total;
		  input(_d, puzzle);
		  list.push_back(_d);
		  pthread_mutex_unlock(&mutex1);
		  pthread_cond_broadcast(&dataready);
		//  cout<<"broadcast  "<<total<<'\n';
		}
	  }
	 // cout<<"reader finished\n";
	  pthread_exit(NULL);
	pthread_cond_destroy(&dataready);
}

void* solver(void*){ 
	while(1){		
			pthread_mutex_lock(&mutex2);
			while(list.empty()){
				//cout<<"total_solved is=="<<total_solved<<"--thread awaits--"<<pthread_self()<<'\n';
				while(total_solved>=1000){
					//cout<<pthread_self()<<"  exit!!!!!!!!!\n";
					pthread_mutex_unlock(&mutex2);//must release it before it quits otherwise causing deadlock 
					pthread_exit(NULL);
				}
				//cout<<pthread_self()<<"awaits-----\n";
				pthread_cond_wait(&dataready, &mutex2);
			}
				Datas *d_ele=list.front();
				list.pop_front();
				pthread_mutex_unlock(&mutex2);
				 if (solve_sudoku_dancing_links( d_ele, 0)) { 
//				    _a=new Ans();
					++total_solved;
//					for(int i=0;i<N;++i){
//						_a->a[i]=d_ele->board[i];
//					results[d_ele->serial]=_a;
//					}
					
				  if (!solved(d_ele)){
				  	assert(0);
				  }
				 }
				  else {
					printf("No: ===");
				  }
	}
	pthread_exit(0);
}
#endif
