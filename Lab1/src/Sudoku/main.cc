#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include<pthread.h>
#include"thread.h"
#include <unistd.h>

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main(int argc, char* argv[])
{
  init_neighbors();
  FILE* fp = fopen(argv[1], "r");
  
  pthread_t file_read;
   int64_t start = now();
  pthread_create(&file_read,NULL,reader,(void*)(fp));

  pthread_t sudoku_solve[4];
  for(int i=0;i<4;++i){
    	pthread_create(&sudoku_solve[i], NULL, solver, NULL);
  }
  pthread_join(file_read,NULL);
   for(int i=0;i<4;++i){
    	pthread_join(sudoku_solve[i], NULL);
  }
    int64_t end = now();
  double sec = (end-start)/1000000.0;
  printf("%f sec %f ms each %d\n", sec, 1000*sec/total, total_solved);
map<int,Ans*>::iterator iter;
for (iter = results.begin();iter != results.end(); iter++){
	cout<<"serial is=="<<iter->first<<'\n';
	for(int i=0;i<N;++i)
        cout << iter->second->q[i];
     cout<<'\n';
     for(int i=0;i<N;++i)
        cout << iter->second->a[i];
      cout<<'\n';
 }


  return 0;
}

