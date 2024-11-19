#include <array>
// #include <chrono>
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
    std::unique_lock<std::mutex> lock(mtx);

    int left = (i + 4) % NUM_PHILOSOPHERS;
    int right = (i + 1) % NUM_PHILOSOPHERS;

    state[i] = THINKING;
    can_eat(left);
    can_eat(right);
  }

  // NOTE: using std::cout or other iostream functions can lead to segementation
  // faults since they are shared resources
  void run(int i) {
    for (int k = 0; k < 10; k++) {
      // print state of each philosopher
      // for (int j = 0; j < NUM_PHILOSOPHERS; j++) {
      //   printf("\t");
      // }
      printf("test");
      // std::cout << "Thread id=" << std::this_thread::get_id() << std::endl;
      // std::cout << "P#" << i << " " << state[i] << std::endl;
      // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

      // Get chopsticks
      // get(i);
      //
      // // Eat with chopsticks
      // // ...
      //
      // put(i);
    }
  }
};

int main() {
  Monitor m;
  std::vector<std::thread> philosophers;

  for (int i = 0; i < 5; i++) {
    philosophers.push_back(std::thread(&Monitor::run, &m, i));
  }

  for (auto &p : philosophers) {
    p.join();
  }

  return 0;
}
