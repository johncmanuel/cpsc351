#include <array>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class Monitor {

private:
  static const int NUM_PHILOSOPHERS = 5;
  enum { THINKING, HUNGRY, EATING } state[NUM_PHILOSOPHERS];
  std::mutex mtx;

  // represent each philosopher as a condition variable
  std::array<std::condition_variable, NUM_PHILOSOPHERS> cv;

  void can_eat(int i) {
    int left = (i + 4) % NUM_PHILOSOPHERS;
    int right = (i + 1) % NUM_PHILOSOPHERS;
    if (state[left] != EATING && state[i] == HUNGRY && state[right] != EATING) {
      state[i] = EATING;
      cv[i].notify_one();
    }
  }

public:
  Monitor() {
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
      state[i] = THINKING;
    }
  }
  void get(int i) {
    std::unique_lock<std::mutex> lock(mtx);
    state[i] = HUNGRY;
    can_eat(i);
    if (state[i] != EATING) {
      cv[i].wait(lock);
    }
  }
  void put(int i) {
    int left = (i + 4) % NUM_PHILOSOPHERS;
    int right = (i + 1) % NUM_PHILOSOPHERS;

    std::unique_lock<std::mutex> lock(mtx);
    state[i] = THINKING;
    can_eat(left);
    can_eat(right);
  }

  constexpr int num_philosophers() { return NUM_PHILOSOPHERS; }

  void run(int i) {
    while (true) {
      // print state of each philosopher
      for (int j = 0; j < NUM_PHILOSOPHERS; j++) {
        printf("\t");
      }
      std::cout << "Thread id=" << std::this_thread::get_id() << std::endl;
    }
  }
};

int main() {
  std::vector<Monitor> m;
  std::vector<std::thread> philosophers;

  for (int i = 0; i < 5; i++) {
    philosophers.push_back(std::thread(&Monitor::run, &m[i], i));
  }

  for (auto &p : philosophers) {
    p.join();
  }

  return 0;
}
