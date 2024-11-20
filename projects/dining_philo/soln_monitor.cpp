#include <array>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#define N 5
#define SLEEP_TIME_MS 1000

class Monitor {

private:
  enum { THINKING, HUNGRY, EATING } state[N];
  std::mutex m;
  int print_count = 0;

  // represent each philosopher as a condition variable
  std::array<std::condition_variable, N> cv;

  void can_eat(int i) {
    int left = (i + 4) % N;
    int right = (i + 1) % N;
    if (state[left] != EATING && state[i] == HUNGRY && state[right] != EATING) {
      state[i] = EATING;
      log_state();
      cv[i].notify_one();
    }
  }

  void log_state() {
    std::cout << "\n";

    for (int i = 0; i < N; i++) {
      std::cout << "P" << i << " ";

      switch (state[i]) {
      case THINKING:
        std::cout << "THINKING   ";
        break;
      case HUNGRY:
        std::cout << "HUNGRY     ";
        break;
      case EATING:
        std::cout << "EATING     ";
        break;
      }
    }

    print_count++;
    if (print_count % 5 == 0)
      std::cout << "\n";
    std::cout.flush();
  }

public:
  Monitor() {
    for (int i = 0; i < N; i++) {
      state[i] = THINKING;
    }
    log_state();
  }

  void get(int i) {
    std::unique_lock<std::mutex> lock(m);
    state[i] = HUNGRY;

    log_state();

    can_eat(i);

    if (state[i] != EATING) {
      cv[i].wait(lock);
    }
  }

  void put(int i) {
    std::unique_lock<std::mutex> lock(m);

    int left = (i + 4) % N;
    int right = (i + 1) % N;

    state[i] = THINKING;

    log_state();

    can_eat(left);
    can_eat(right);
  }

  void run(int i) {
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MS));
      get(i);
      std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MS));
      put(i);
    }
  }
};

int main() {
  Monitor m;
  std::vector<std::thread> philosophers;

  for (int i = 0; i < N; i++) {
    philosophers.push_back(std::thread(&Monitor::run, &m, i));
  }

  for (auto &p : philosophers) {
    p.join();
  }

  return 0;
}
