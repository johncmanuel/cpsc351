#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "common_threads.h"

// Threaded programming == indetermant, can't always get the same result
// every time

volatile int counter = 0;
int loops;
pthread_mutex_t lock;

// void* = take any type, return any type
void* worker(void* arg) {
  int i;
  for (i = 0; i < loops; ++i) {
    Pthread_mutex_lock(&lock);
    ++counter;
    Pthread_mutex_unlock(&lock);
  }
  return NULL;
}

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: threads <value>\n");
    exit(1);
  }
  loops = atoi(argv[1]);
  pthread_t p1, p2;
  printf("Initial value: %d\n", counter);
  Pthread_create(&p1, NULL, worker, NULL);
  Pthread_create(&p2, NULL, worker, NULL);

  Pthread_join(p1, NULL);
  Pthread_join(p2, NULL);
  printf("Final value   : %d\n", counter);
  return 0;
}