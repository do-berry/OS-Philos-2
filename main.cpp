#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <thread>
#include <string>
#include <cstring>
#include <vector>
#include <ncurses.h>
#include <condition_variable>
#include <curses.h>

#include "Filozof.h"

using namespace std;

#define ileFilozofow 5

struct xy {
  int x;
  int y;
};

mutex watkiMutex;
struct xy wspolrzedne [ileFilozofow];
struct xy sztucce;
WINDOW * w[ileFilozofow];
thread watki [ileFilozofow + 1];
thread czasy [ileFilozofow];
Filozof * filozofowie [ileFilozofow];
condition_variable cv;
bool zakonczony = false;

void * liczCzas(int mili, int id) {
  for (int i = mili; i >= 0; i--) {
    string czas = to_string(i) + "\n";
    watkiMutex.lock();
    mvwaddstr(w[id], 5, 0, czas.c_str());
    wrefresh(w[id]);
    watkiMutex.unlock();
  }
  zakonczony = true;
}

void * robCos(int id) {
  unsigned int timeout = rand() % 5 + 2;             // tyle bedzie spal watek
  unsigned int ileRazy = rand() % 10 + 2;                  // ile razy bedzie jadl/filozofowal
  string tekst;

  for (int i = 0; i < ileRazy; i++) {
    watkiMutex.lock();
    if (filozofowie[id]->posilek) {
      tekst = "je\n";
      mvwaddstr(w[id], 1, 0, tekst.c_str());
      wrefresh(w[id]);
      filozofowie[id]->filozofuj();
    } else {
      tekst = "filozofuje\n";
      mvwprintw(w[id], 1, 0, tekst.c_str());
      wrefresh(w[id]);
      filozofowie[id]->jedz();
    }
    watkiMutex.unlock();
    //this_thread::sleep_for(chrono::seconds(timeout));
    unique_lock<mutex> lck(watkiMutex);
    while (!zakonczony) cv.wait(lck);
    cv.notify_one();
    zakonczony = false;
  }
}

void sprawdzStatus(int id) {
  watkiMutex.lock();
  if (!Filozof::lewyWidelec) {
    mvwaddstr(w[id], 1, 0, "lewy widelec");
  } else {
    mvwaddstr(w[id], 1, 0, "\n");
  }
  wrefresh(w[id]);
  if (!Filozof::prawyWidelec) {
    mvwaddstr(w[id], 2, 0, "prawy widelec");
  } else {
    mvwaddstr(w[id], 2, 0, "\n");
  }
  wrefresh(w[id]);
  watkiMutex.unlock();
}

void utworzWatki(int x, int y) {
  for (int i = 0; i < ileFilozofow; i++) {
    string tekst = "Filozof " + to_string(i) + ":\n";
    filozofowie[i] = new Filozof();                       // id watku odpowiada
    mvwaddstr(w[i], 0, 0, tekst.c_str());
    mvwprintw(w[i], 4, 0, "Czas:\n");
    wrefresh(w[i]);
  }
  mvwaddstr(w[5], 0, 0, "Na stole:\n");
  wrefresh(w[5]);

  for (int i = 0; i <= ileFilozofow; i++) {
    if (i == ileFilozofow) {
      watki[i] = thread(&sprawdzStatus, i);
    } else {
      watki[i] = thread(&robCos, i);                        // id filozofa
    }
  }
}

void zatrzymajWatki() {
  for (int i = 0; i <= ileFilozofow; i++) {
    watki[i].join();
  }
}

void start() {
  int x, y;

  initscr();
  refresh();

  getmaxyx(stdscr, y, x);
  x /= 3;
  y /= 2;
  w[0] = newwin(y, x, 0, 0);
  w[1] = newwin(y, x, 0, x);
  w[2] = newwin(y, x, 0, 2*x);
  w[3] = newwin(y, x, y, 0);
  w[4] = newwin(y, x, y, x);
  w[5] = newwin(y, x, y, 2*x);

  utworzWatki(x, y);

  char wybor = 0;
  while(1) {
    if ((wybor = getch()) == 'n'){
      break;
    }
  }
  cout << "Koniec" << endl;
  endwin();
}

int main() {
  start();
  //utworzWatki();
  return 0;
}
