#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

enum PhilosopherState { THINKING, HUNGRY, EATING };

// Create a semaphore for each philosopher and a global lock
sem_t mutex;
sem_t semaphore_philo[NUM_PHILOSOPHERS];

// Track global state of each philosopher
int state[NUM_PHILOSOPHERS];

void perform_check(int i) {
  if (state[i] == HUNGRY) {
  }
}

// Thread function representing the philosopher
void *run_philosopher(void *arg) {
  int i = *(int *)arg;

  while (1) {
    printf("Philosopher %d is thinking\n", i);
    sleep(1);

    printf("Philosopher %d is hungry\n", i);
    // Pick up fork when hungry
    sem_wait(&mutex);
    state[i] = HUNGRY;
    perform_check(i);
    sem_post(&mutex);
    sem_wait(&semaphore_philo[i]);
  }
}

int main() {
  pthread_t threads[NUM_PHILOSOPHERS];
  int philo_id[NUM_PHILOSOPHERS];

  // Initialize semaphores and philosopher state
  sem_init(&mutex, 0, 1);
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    sem_init(&semaphore_philo[i], 0, 0);
    state[i] = THINKING;
  }

  // Create threads
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    philo_id[i] = i;
    pthread_create(&threads[i], NULL, run_philosopher, (void *)&philo_id[i]);
  }

  // Perform waits on threads
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
