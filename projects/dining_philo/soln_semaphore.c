#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

enum { THINKING, HUNGRY, EATING } state[NUM_PHILOSOPHERS];
sem_t chopsticks[NUM_PHILOSOPHERS];
sem_t mutex;

void log_states() {
  printf("\n");
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    printf("P%d ", i);
    switch (state[i]) {
    case THINKING:
      printf("THINKING   ");
      break;
    case HUNGRY:
      printf("HUNGRY     ");
      break;
    case EATING:
      printf("EATING     ");
      break;
    }
  }
  fflush(stdout);
}

void can_eat(int i) {
  if (state[i] == HUNGRY && state[(i + 1) % NUM_PHILOSOPHERS] != EATING &&
      state[(i + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS] != EATING) {
    state[i] = EATING;
    log_states();
    sem_post(&chopsticks[i]);
  }
}

void pick_up(int i) {
  sem_wait(&mutex);
  state[i] = HUNGRY;
  log_states();
  sem_post(&mutex);
  sem_wait(&chopsticks[i]);
}

void put_down(int i) {
  sem_wait(&mutex);
  state[i] = THINKING;
  log_states();
  can_eat((i + 1) % NUM_PHILOSOPHERS);
  can_eat((i + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS);
  sem_post(&mutex);
}

void *run(void *arg) {
  int i = *(int *)arg;

  while (1) {
    sleep(1);
    pick_up(i);
    sleep(1);
    put_down(i);
  }

  return NULL;
}

int main() {
  pthread_t philosophers[NUM_PHILOSOPHERS];
  int philo_id[NUM_PHILOSOPHERS];

  sem_init(&mutex, 0, 1);
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    sem_init(&chopsticks[i], 0, 0);
    state[i] = THINKING;
  }

  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    philo_id[i] = i;
    pthread_create(&philosophers[i], NULL, run, &philo_id[i]);
  }

  log_states();

  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    pthread_join(philosophers[i], NULL);
  }

  sem_destroy(&mutex);
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    sem_destroy(&chopsticks[i]);
  }

  return 0;
}
