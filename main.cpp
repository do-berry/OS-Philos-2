#include <iostream>
#include <mutex>
#include <thread>
#include <string>
#include <chrono>
#include <ncurses.h>
#include <atomic>
#include "Philo.h"

using namespace std;

#define howMany 5

struct Fork {
  mutex forkMutex;
  bool free = true;
};

mutex threadMutex;
Fork forks[5];
WINDOW * windows[howMany];
thread * threads[howMany];
Philo * philos[howMany];
int forkList[howMany][howMany] = {{0,1},{1,2},{2,3},{3,4},{4,0}};
static atomic<bool> run (false);

void setTo(int f, bool value) {
  forks[f].forkMutex.lock();
  forks[f].free = value;
  forks[f].forkMutex.unlock();
}

void write(int id, int line, string txt) {
  threadMutex.lock();
  mvwaddstr(windows[id], line, 0, txt.c_str());
  wrefresh(windows[id]);
  threadMutex.unlock();
}

void writeTimeOfEating(int id) {
  string txt;
  int i = 6;
  while (i > 0 && run) {
    txt = "Time of eating: " + to_string(i*500) + "\n";
    write(id, 6, txt);
    i--;
    this_thread::sleep_for(chrono::milliseconds(500));
  }
}

void * doSomething(int id) {
  while (run) {
    int left = forkList[id][0];
    int right = forkList[id][1];
    if (left < right) {
      if (forks[left].free) {
        setTo(left, false);
        write(id, 3, "left fork\n");
        if (forks[right].free) {
          bool stopEating = false;
          setTo(right, false);
          write(id, 1, "eats\n");
          write(id, 4, "right fork\n");
          writeTimeOfEating(id);
          setTo(right, true);
          write(id, 4, "\n");
        } else {
          write(id, 1, "thinks\n");
          write(id, 6, "\n");
        }
        setTo(left, true);
        write(id, 3, "\n");
      } else {
        write(id, 1, "thinks\n");
        write(id, 6, "\n");
      }
    } else {
      if (forks[right].free) {
        setTo(right, false);
        write(id, 3, "right fork\n");
        if (forks[left].free) {
          setTo(left, false);
          write(id, 1, "eats\n");
          write(id, 4, "left fork\n");
          writeTimeOfEating(id);
          setTo(left, true);
          write(id, 4, "\n");
        } else {
          write(id, 1, "thinks\n");
          write(id, 6, "\n");
        }
        setTo(right, true);
        write(id, 3, "\n");
      } else {
        write(id, 1, "thinks\n");
        write(id, 6, "\n");
      }
    }
  }
}

void createThreads() {
  for (int i = 0; i < howMany; i++) {
    philos[i] = new Philo();
    threads[i] = new thread(doSomething, i);
  }
}

void stopThreads() {
  for (int i = 0; i < howMany; i++) {
    threads[i]->join();
    delete threads[i];
    delete philos[i];
  }
}

void start() {
  int x, y;
  initscr();
  refresh();
  getmaxyx(stdscr, y, x);
  x /= 3;
  y /= 2;
  windows[0] = newwin(y, x, 0, 0);
  windows[1] = newwin(y, x, 0, x);
  windows[2] = newwin(y, x, 0, 2*x);
  windows[3] = newwin(y, x, y, 0);
  windows[4] = newwin(y, x, y, x);
  for (int i = 0; i < howMany; i++) {
    string txt = "Philosopher " + to_string(i) + ":\n";
    mvwaddstr(windows[i], 0, 0, txt.c_str());
    wrefresh(windows[i]);
  }
  char choice = 0;
  createThreads();
  while (run) {
    cin >> choice;
    if (choice == 'n') {
      run.store(false);
      stopThreads();
    }
  }
  endwin();
}

int main() {
  run.store(true);
  start();
  return 0;
}
