#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

enum { THINKING, HUNGRY, EATING, WAITING } state[NUM_PHILOSOPHERS];
sem_t chopsticks[NUM_PHILOSOPHERS];
sem_t mutex;

void can_eat(int i) {}

void think(int i) {
  printf("P#%d THINKING\n", i);
  sleep(1);
}

void pick_up_chopsticks(int i) {
  sem_wait(&mutex);
  state[i] = HUNGRY;
  sem_post(&mutex);
  sem_wait(&chopsticks[i]);
}

void put_down_chopsticks(int i) {}

void *run_philosopher(void *arg) {
  int i = *(int *)arg;
  int left_neighbor = i;
  int right_neighbor = (i + 1) % NUM_PHILOSOPHERS;

  while (1) {
    think(i);

    // Hungry state
    if (left_neighbor % 2 == 0) {
      sem_wait(&chopsticks[left_neighbor]);
      printf("P#%d picked up left chopstick", i);
      sem_wait(&chopsticks[right_neighbor]);
      printf("P#%d picked up right chopstick", i);
    } else {
      sem_wait(&chopsticks[right_neighbor]);
      sem_wait(&chopsticks[left_neighbor]);
    }

    // Eating state, so sleep for 1 second
    sleep(1);

    sem_post(&chopsticks[left_neighbor]);
    sem_post(&chopsticks[right_neighbor]);
  }

  return NULL;
}

int main() {
  pthread_t philosophers[NUM_PHILOSOPHERS];
  int philo_id[NUM_PHILOSOPHERS];

  sem_init(&mutex, 0, 1);
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    sem_init(&chopsticks[i], 0, 0);
  }

  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    philo_id[i] = i;
    pthread_create(&philosophers[i], NULL, run_philosopher,
                   (void *)&philo_id[i]);
  }

  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    pthread_join(philosophers[i], NULL);
  }

  sem_destroy(&mutex);
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    sem_destroy(&chopsticks[i]);
  }

  return 0;
}
