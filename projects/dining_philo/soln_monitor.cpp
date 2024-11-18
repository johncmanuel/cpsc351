#include <array>
#include <thread>

class Monitor {

private:
  enum { THINKING, HUNGRY, EATING } state[5];
  void can_eat(int i) {
    if (state[(i + 4) % 5] != EATING && state[i] == HUNGRY &&
        state[(i + 1) % 5] != EATING) {
      state[i] = EATING;
      // signal state[i]
      // state[i].signal(), something like that
    }
  }

public:
  Monitor() {
    for (int i = 0; i < 5; i++) {
      state[i] = THINKING;
    }
  }
  void get(int i) {
    state[i] = HUNGRY;
    can_eat(i);
    if (state[i] != EATING) {
      // wait state[i]
      // state[i].wait()
    }
  }
  void put(int i) {
    state[i] = THINKING;
    can_eat((i + 4) % 5);
    can_eat((i + 1) % 5);
  }
};

int main() { std::array<std::thread, 5> philosophers; }
