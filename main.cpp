#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <thread>

#include "Filozof.h"

using namespace std;

#define ileFilozofow 5

mutex watkiMutex;
thread watki [ileFilozofow];
Filozof * filozofowie [ileFilozofow];

void * robCos(int id) {
  unsigned int timeout = rand() % 5000 + 2500;             // tyle bedzie spal watek
  unsigned int ileRazy = rand() % 10 + 2;                  // ile razy bedzie jadl/filozofowal

  for (int i = 0; i < ileRazy; i++) {
    watkiMutex.lock();
    cout << "Filozof " << id;
    if (filozofowie[id]->posilek) {
      cout << " je." << endl;
      filozofowie[id]->filozofuj();
    } else {
      cout << " filozofuje." << endl;
      filozofowie[id]->jedz();
    }
    watkiMutex.unlock();
    this_thread::sleep_for(chrono::milliseconds(timeout));
  }
}

void utworzWatki() {
  for (int i = 0; i < ileFilozofow; i++) {
    filozofowie[i] = new Filozof();                       // id watku odpowiada
    watki[i] = thread(&robCos, i);                        // id filozofa
  }
}

void zatrzymajWatki() {
  for (int i = 0; i < ileFilozofow; i++) {
    watki[i].join();
  }
}

int main() {
  srand(time(NULL));

  utworzWatki();
  zatrzymajWatki();

  return 0;
}

