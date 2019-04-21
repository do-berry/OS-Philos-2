#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <thread>
#include <string>
#include <cstring>
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
Filozof * filozofowie [ileFilozofow];
bool run = false;
condition_variable cv;
bool ready = false;

void * robCos(int id) {
  unsigned int timeout = rand() % 6000 + 2500;             // tyle bedzie spal watek
  string tekst, nr;
  nr = "Filozof " + to_string(id) + "\n";

  while (run) {
    if (filozofowie[id]->posilek) {
      if (!Filozof::lewyWidelec) {
        Filozof::lewyWidelec = true;
        tekst = "je lewym widelcem";
        watkiMutex.lock();
        mvwaddstr(w[id], 1, 0, tekst.c_str());
        wrefresh(w[id]);
        mvwaddstr(w[ileFilozofow], 1, 0, nr.c_str());
        wrefresh(w[ileFilozofow]);
        watkiMutex.unlock();
        this_thread::sleep_for(chrono::milliseconds(timeout));
        Filozof::lewyWidelec = false;
      } else if (!Filozof::prawyWidelec) {
        Filozof::prawyWidelec = true;
        tekst = "je prawym widelcem";
        watkiMutex.lock();
        mvwaddstr(w[id], 1, 0, tekst.c_str());
        wrefresh(w[id]);
        mvwaddstr(w[ileFilozofow], 2, 0, nr.c_str());
        wrefresh(w[ileFilozofow]);
        watkiMutex.unlock();
        this_thread::sleep_for(chrono::milliseconds(timeout));
        Filozof::prawyWidelec = false;
      } else {
        tekst = "nie ma wolnego widelca";
        watkiMutex.lock();
        mvwaddstr(w[id], 1, 0, tekst.c_str());
        wrefresh(w[id]);
        watkiMutex.unlock();
        this_thread::sleep_for(chrono::milliseconds(1500));
      }
      filozofowie[id]->filozofuj();
    } else {
      tekst = "filozofuje\n";
      watkiMutex.lock();
      mvwprintw(w[id], 1, 0, tekst.c_str());
      wrefresh(w[id]);
      watkiMutex.unlock();
      filozofowie[id]->jedz();
    }
  }
}

// true: lewy; false: prawy;
void sprawdzStatus(int id) {
  watkiMutex.lock();
  if (!Filozof::lewyWidelec) {
    mvwaddstr(w[id], 1, 0, "lewy widelec");
  } else {
    //mvwaddstr(w[id], 1, 0, "\n");
  }
  wrefresh(w[id]);
  if (!Filozof::prawyWidelec) {
    mvwaddstr(w[id], 2, 0, "prawy widelec");
  } else {
    //mvwaddstr(w[id], 2, 0, "\n");
  }
  wrefresh(w[id]);
  watkiMutex.unlock();
}

void utworzWatki(int x, int y) {
  run = true;
  for (int i = 0; i < ileFilozofow; i++) {
    string tekst = "Filozof " + to_string(i) + ":\n";
    filozofowie[i] = new Filozof();                       // id watku odpowiada
    mvwaddstr(w[i], 0, 0, tekst.c_str());
    wrefresh(w[i]);
  }
  mvwaddstr(w[5], 0, 0, "Przy stole:\n");
  wrefresh(w[5]);

  for (int i = 0; i < ileFilozofow; i++) {
    watki[i] = thread(&robCos, i);                        // id filozofa
  }
  watki[ileFilozofow] = thread(&sprawdzStatus, ileFilozofow);
}

void zatrzymajWatki() {
  run = false;
  for (int i = 0; i < ileFilozofow; i++) {
    watki[i].join();
    watki[i].detach();
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

  char wybor = 0;
  utworzWatki(x, y);

  while(1) {
    if ((wybor = getch()) == 'n'){
      zatrzymajWatki();
      break;
    }
  }
  endwin();
}

int main() {
  start();
  return 0;
}
