#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include<iostream>
using namespace std;
#include"dance.h"
#include<deque>
#include<map>

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}
struct Ques{
	int serial;
	char puz[128];
};
struct Ans{
	int serial;
	int answer[N];
};
int total;
int total_solved;
deque<Ques>list;
map<int, Ans>results;
pthread_cond_t dataready;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER, mutex2=PTHREAD_MUTEX_INITIALIZER,mutex3=PTHREAD_MUTEX_INITIALIZER;
void* reader(void* temp){
	FILE* fp =(FILE*)temp;
	char puzzle[128];
  while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
    if (strlen(puzzle) >= N) {
   	 //pthread_mutex_lock(&mutex1);
      ++total;
      Ques q;
      q.serial=total;
      memcpy(q.puz,puzzle,sizeof puzzle);
      list.push_back(q);  
      //pthread_mutex_unlock(&mutex1); 
      pthread_cond_broadcast(&dataready);
    }
  }
  pthread_exit(NULL);
}
void* solver(void*){
	while(1){
			pthread_mutex_lock(&mutex2);
			while(list.empty()){
				if(total_solved>=1000){
				pthread_mutex_unlock(&mutex2);
				pthread_exit(NULL);
				}
				pthread_cond_wait(&dataready,&mutex2);
			}
			Ans a;
			a.serial=list.front().serial;
			for(int i=0;i<N;++i){
				a.answer[i]=list.front().puz[i]-'0';
			}
			++total_solved;
			list.pop_front(); 
			Dance dan(a.answer);
			pthread_mutex_unlock(&mutex2);
		  if (dan.solve()) {
		    results[a.serial]=a;
		    if (!dan.solved())
		      assert(0);
		  }
		  else {
		  printf("No:");
		  for(int i=0;i<N;i++){
		  cout<<dan.board[i];
		  }
		  cout<<'\n';
		  }
		//pthread_mutex_unlock(&mutex2);
     }
}
int main(int argc, char* argv[])
{
  FILE* fp = fopen(argv[1], "r");

  int64_t start = now();
  pthread_cond_init(&dataready, NULL);
  pthread_t file_read;
  pthread_create(&file_read,NULL,reader,(void*)(fp)); 
  
  pthread_t sudoku_solve[4];
 
  for(int i=0;i<4;++i){
    	pthread_create(&sudoku_solve[i], NULL, solver, NULL);
  }
  
   for(int i=0;i<4;++i){
    	pthread_join(sudoku_solve[i], NULL);
  }
  pthread_join(file_read,NULL);
  int64_t end = now();
  double sec = (end-start)/1000000.0;
  printf("%f sec %f ms each %d\n", sec, 1000*sec/total, total_solved);
// map<int,Ans>::iterator iter;
//for (iter = results.begin();iter != results.end(); iter++){
//	cout<<"serial is=="<<iter->first<<'\n';
//     for(int i=0;i<N;++i)
//        cout << iter->second.answer[i];
//      cout<<'\n';
// }

  return 0;
}

