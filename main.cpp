#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <thread>
#include <string>
#include <cstring>
#include <chrono>
#include <ncurses.h>
#include <condition_variable>
#include <curses.h>
#include <atomic>
#include <functional>
#include "Philo.h"

using namespace std;

#define howMany 5

struct Fork {
  mutex mtx;
  bool free = true;
};

mutex mtx;
Fork forks[5];
WINDOW * w[howMany];
thread threads[howMany];
Philo * philos[howMany];
int forkList[howMany][howMany] = {{0,1},{1,2},{2,3},{3,4},{4,0}};
unsigned int dur = 3;
condition_variable cv;
bool isFree(int f);
static atomic<bool> run (false);

bool isFree(int f) {
  if (forks[f].free) return true;
  else return false;
}

void setTo(int f, bool what) {
  forks[f].mtx.lock();
  forks[f].free = what;
  forks[f].mtx.unlock();
}

void write(int id, int line, char txt[]) {
  mtx.lock();
  mvwaddstr(w[id], line, 0, txt);
  wrefresh(w[id]);
  mtx.unlock();
}

void * doSth(int id) {
  while (run) {
    int left = forkList[id][0];
    int right = forkList[id][1];
    if (left < right) {
      if (forks[left].free) {
        setTo(left, false);
        write(id, 3, "left fork\n");
        if (forks[right].free) {
          setTo(right, false);
          write(id, 1, "eats\n");
          write(id, 4, "right fork\n");
          this_thread::sleep_for(chrono::seconds(dur));
          setTo(right, true);
          write(id, 4, "\n");
        } else {
          write(id, 1, "thinks\n");
        }
        setTo(left, true);
        write(id, 3, "\n");
      } else {
        write(id, 1, "thinks\n");
      }
    } else {
      if (forks[right].free) {
        setTo(right, false);
        write(id, 3, "right fork\n");
        if (forks[left].free) {
          setTo(left, false);
          write(id, 1, "eats\n");
          write(id, 4, "left fork\n");
          this_thread::sleep_for(chrono::seconds(dur));
          setTo(left, true);
          write(id, 4, "\n");
        } else {
          write(id, 1, "thinks\n");
        }
        setTo(right, true);
        write(id, 3, "\n");
      } else {
        write(id, 1, "thinks\n");\
      }
    }
  }
}

void createThreads() {
  for (int i = 0; i < howMany; i++) {
    philos[i] = new Philo();
    threads[i] = thread(doSth, i);           // i of thread == i of philo
  }
}

void stopThreads() {
  for (int i = 0; i < howMany; i++) {
    threads[i].join();
  }
}

void start() {
  int x, y;
  initscr();
  refresh();
  getmaxyx(stdscr, y, x);
  x /= 3;
  y /= 2;
  w[0] = newwin(y, x, 0, 0);            // filozof 0
  w[1] = newwin(y, x, 0, x);            // filozof 1
  w[2] = newwin(y, x, 0, 2*x);          // filozof 2
  w[3] = newwin(y, x, y, 0);            // filozof 3
  w[4] = newwin(y, x, y, x);            // filozof 4
  // w[5] = newwin(y, x, y, 2*x);          // kto przy stole
  for (int i = 0; i < howMany; i++) {
    string txt = "Philosopher " + to_string(i) + ":\n";
    mvwaddstr(w[i], 0, 0, txt.c_str());
    wrefresh(w[i]);
  }
  char choice = 0;
  createThreads();
  while (run) {
    cin >> choice;
    if (choice == 'n') {
      run = false;
      stopThreads();
    }
  }
  endwin();
}

int main() {
  run = true;
  start();
  return 0;
}
