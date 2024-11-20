#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

enum { THINKING, HUNGRY, EATING } state[NUM_PHILOSOPHERS];
sem_t chopsticks[NUM_PHILOSOPHERS];
sem_t m;
int print_count = 0;

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

  print_count++;
  if (print_count % 5 == 0)
    printf("\n");
  fflush(stdout);
}

void can_eat(int i) {
  int left = (i + 4) % NUM_PHILOSOPHERS;
  int right = (i + 1) % NUM_PHILOSOPHERS;

  if (state[i] == HUNGRY && state[left] != EATING && state[right] != EATING) {
    state[i] = EATING;
    log_states();
    sem_post(&chopsticks[i]);
  }
}

void pick_up(int i) {
  sem_wait(&m);
  state[i] = HUNGRY;
  log_states();
  sem_post(&m);
  sem_wait(&chopsticks[i]);
}

void put_down(int i) {
  sem_wait(&m);

  int left = (i + 4) % NUM_PHILOSOPHERS;
  int right = (i + 1) % NUM_PHILOSOPHERS;
  state[i] = THINKING;

  log_states();
  can_eat(left);
  can_eat(right);
  sem_post(&m);
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

  sem_init(&m, 0, 1);
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

  sem_destroy(&m);
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    sem_destroy(&chopsticks[i]);
  }

  return 0;
}
