#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

enum PhilosopherState { THINKING, HUNGRY, EATING };

// Create a semaphore for each philosopher and a global lock
sem_t mutex;
sem_t forks[NUM_PHILOSOPHERS];

// Track global state of each philosopher
int state[NUM_PHILOSOPHERS];

// Thread function representing the philosopher
void *run_philosopher(void *arg) {
  int i = *(int *)arg;

  while (1) {
    // Pick up fork when hungry
    sem_wait(&mutex);
    state[i] = HUNGRY;
    sem_post(&mutex);
    sem_wait(&forks[i]);

    printf("Philosopher %d is eating\n", i);
    sleep(1);

    // Put down fork when done eating
  }
}

int main() {
  pthread_t philosophers[NUM_PHILOSOPHERS];
  int philo_id[NUM_PHILOSOPHERS];

  // Initialize semaphores and philosopher state
  sem_init(&mutex, 0, 1);
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    sem_init(&forks[i], 0, 0);
    state[i] = THINKING;
  }

  // Create threads
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    philo_id[i] = i;
    pthread_create(&philosophers[i], NULL, run_philosopher,
                   (void *)&philo_id[i]);
  }

  // Perform waits on threads
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    pthread_join(philosophers[i], NULL);
  }

  // Destroy mutex and semaphores
  sem_destroy(&mutex);
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    sem_destroy(&forks[i]);
  }

  return 0;
}
