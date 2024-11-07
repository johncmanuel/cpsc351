#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum PhilosopherState { THINKING, HUNGRY, EATING };

const int NUM_PHILOSOPHERS = 5;

// Create a semaphore for each philosopher
sem_t mutex;
sem_t s[NUM_PHILOSOPHERS];

// Track state of each philosopher
int state[NUM_PHILOSOPHERS];

// Thread function representing the philosopher
void *run_philosopher(void *arg) {
  int i = *(int *)arg;

  while (1) {
    printf("Philosopher %d is thinking\n", i);
    sleep(1);

    printf("Philosopher %d is hungry\n", i);
    // Pick up fork
    sem_wait(&mutex);
    state[i] = HUNGRY;
    sem_post(&mutex);
    sem_wait(&s[i]);
  }
}

int main() {
  pthread_t threads[NUM_PHILOSOPHERS];

  // Initialize semaphores and philosopher state
  sem_init(&mutex, 0, 1);
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    sem_init(&s[i], 0, 0);
    state[i] = THINKING;
  }

  // Create threads
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    pthread_create(&threads[i], NULL, run_philosopher, (void *)&i);
  }

  return 0;
}
