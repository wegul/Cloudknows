#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include<stdlib.h>
#include"dance.h"
#include<deque>
#include <sys/sysinfo.h>
#include<fstream>
#include<map>
int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

int limit=0;
int total;
int total_solved;
deque<Ques>list;
map<int, Ans>results;
pthread_cond_t dataready;
pthread_mutex_t mutex2=PTHREAD_MUTEX_INITIALIZER,mutex3=PTHREAD_MUTEX_INITIALIZER;
void* reader(void* temp){
	FILE* fp =(FILE*)temp;
	char puzzle[128];
  while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
    if (strlen(puzzle) >= N) {
      ++total;
      Ques q;
      q.serial=total;
      memcpy(q.puz,puzzle,sizeof puzzle);
      list.push_back(q);  
      pthread_cond_broadcast(&dataready);
    }
  }
  pthread_exit(NULL);
}
void* solver(void*){
	while(1){
			pthread_mutex_lock(&mutex2);
			while(list.empty()){
				if(total_solved>=limit){
				pthread_mutex_unlock(&mutex2);
				pthread_mutex_unlock(&mutex3);
				pthread_exit(NULL);
				}
				pthread_cond_wait(&dataready,&mutex2);
			}
		    Ques top=list.front();	
		 	list.pop_front(); 
			++total_solved;
			pthread_mutex_unlock(&mutex2);
		  int tempo[N];
		  for(int i=0;i<N;++i){
				tempo[i]=top.puz[i]-'0';
			}
			Dance dan(tempo);
			dan.serial=top.serial;
			dan.solve();
			Ans a;
            a.serial=dan.serial;
            for(int i=0;i<N;++i){
            	a.answer[i]=dan.inout_[i];
            }
           pthread_mutex_lock(&mutex3);
            results[a.serial]=a;
            pthread_mutex_unlock(&mutex3);
     }
}

int main()
{		
		int nprocs = get_nprocs_conf ();
		const int PROC=nprocs; 
		char *name;
	while(1){
		name=(char *)malloc(sizeof(char));//must malloc space  before use!!!!
		total=0;
		total_solved=0;
	  cin>>name;
	  
	  limit=atoi(&name[6]);
	  FILE* fp = fopen(name, "r");
		
	  pthread_cond_init(&dataready, NULL);
	  pthread_t file_read; 
	  pthread_t sudoku_solve[PROC];
	  
	   int64_t start = now();
	   //========================
	  pthread_create(&file_read,NULL,reader,(void*)(fp)); 
	  for(int i=0;i<PROC;++i){
			pthread_create(&sudoku_solve[i], NULL, solver, NULL);
	  }
	  	pthread_join(sudoku_solve[0], NULL);
	  //======================
	  int64_t end = now(); 
	  
	  for(int i=1;i<=limit;++i){
	  	 for(int j=0;j<N;++j){
				cout <<results[i].answer[j];
		    }
		  cout<<'\n';
	  }
	  double sec = (end-start)/1000000.0;
	  //printf("%f sec %f ms each %d\n", sec, 1000*sec/total, total_solved);
	}
  return 0;
}
