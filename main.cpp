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

mutex watkiMutex;
int posilki [ileFilozofow] = {3, 3, 3, 3, 3};
WINDOW * w[ileFilozofow];
thread watki [ileFilozofow + 1];
condition_variable cv;
Filozof * filozofowie [ileFilozofow];
bool run = true;

bool koniec() {
  if (run) return false;
  else return true;
}

void pisz(int id, int linia, string tekst) {
  watkiMutex.lock();
  mvwaddstr(w[id], linia, 0, tekst.c_str());
  wrefresh(w[id]);
  watkiMutex.unlock();
}

void ilePosilkow(int id) {
  string tekst;
  if (posilki[id] > 0)
    tekst = "zjedzonych posilkow: " + to_string(posilki[id]) + "\n";
  else {
    tekst = "umarl z glodu\n";
  }
  watkiMutex.lock();
  mvwaddstr(w[id], 4, 0, tekst.c_str());
  wrefresh(w[id]);
  watkiMutex.unlock();
  if (posilki[id] == 0) {
    unique_lock<mutex> lck(watkiMutex);
    cv.wait(lck, koniec);
  }
}

void * robCos(int id) {
  unsigned int timeout = rand() % 6000 + 2500;             // tyle bedzie spal watek
  string tekst, nr;
  nr = "Filozof " + to_string(id) + "\n";

  while (run) {
    ilePosilkow(id);
    if (filozofowie[id]->posilek) {
      tekst = "jest glodny";
      pisz(id, 1, tekst);
      this_thread::sleep_for(chrono::milliseconds(1000));
      if (!Filozof::lewyWidelec) {
        Filozof::zmien(true, true);
        tekst = "je lewym widelcem";
        posilki[id]++;
        pisz(id, 1, tekst);
        pisz(ileFilozofow, 1, nr);
        this_thread::sleep_for(chrono::milliseconds(timeout));
        Filozof::zmien(true, false);
      } else if (!Filozof::prawyWidelec) {
        Filozof::zmien(false, true);
        tekst = "je prawym widelcem";
        posilki[id]++;
        pisz(id, 1, tekst);
        pisz(ileFilozofow, 2, nr);
        this_thread::sleep_for(chrono::milliseconds(timeout));
        Filozof::zmien(false, false);
      } else {
        tekst = "nie ma wolnego widelca";
        posilki[id]--;
        pisz(id, 1, tekst);
        this_thread::sleep_for(chrono::milliseconds(1000));
      }
      filozofowie[id]->filozofuj();
    } else {
      tekst = "filozofuje\n";
      pisz(id, 1, tekst);
      this_thread::sleep_for(chrono::milliseconds(timeout));
      filozofowie[id]->jedz();
    }
  }
}

void sprawdzStatus(int id) {
  watkiMutex.lock();
  if (!Filozof::lewyWidelec) {
    mvwaddstr(w[id], 1, 0, "lewy widelec");
  }
  if (!Filozof::prawyWidelec) {
    mvwaddstr(w[id], 2, 0, "prawy widelec");
  }
  wrefresh(w[id]);
  watkiMutex.unlock();
}

void utworzWatki(int x, int y) {
  run = true;
  for (int i = 0; i < ileFilozofow; i++) {
    string tekst = "Filozof " + to_string(i) + ":\n";
    filozofowie[i] = new Filozof();
    mvwaddstr(w[i], 0, 0, tekst.c_str());
    wrefresh(w[i]);
  }
  string txt = "Przy stole:\n";
  mvwaddstr(w[ileFilozofow], 0, 0, txt.c_str());
  wrefresh(w[ileFilozofow]);
  for (int i = 0; i < ileFilozofow; i++) {
    watki[i] = thread(&robCos, i);                        // id filozofa
  }
//  watki[ileFilozofow] = thread(&sprawdzStatus, ileFilozofow);*/
}

void zatrzymajWatki() {
  run = false;
  for (int i = 0; i < ileFilozofow; i++) {
    //if (watki[i].joinable()) {
      watki[i].join();
    //}
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
  w[5] = newwin(y, x, y, 2*x);          // kto przy stole
  char wybor = 0;
  utworzWatki(x, y);
  while(wybor != 'n') {
      cin >> wybor;
      if (wybor == 'n') {
        zatrzymajWatki();
      }
  }
  endwin();
}

int main() {
  start();
  return 0;
}
