#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include<pthread.h>
#include"thread.h"

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
  pthread_create(&file_read,NULL,reader,(void*)(fp));
  
  pthread_t sudoku_solve;
  pthread_create(&sudoku_solve, NULL, solver, NULL);

  int64_t start = now();

  int64_t end = now();
  double sec = (end-start)/1000000.0;
  printf("%f sec %f ms each \ntotal=%d \n", sec, 1000*sec/total, total_solved);

  return 0;
}

