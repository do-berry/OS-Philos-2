#ifndef FILOZOF_H
#define FILOZOF_H

#include <cstdlib>
#include <ctime>
#include <curses.h>

class Filozof {
private:
public:
  static int licznik;
  static bool lewyWidelec;                        // false == wolny
  static bool prawyWidelec;                       // true == zajety
  bool posilek;
  bool filozofowanie;
  Filozof();
  ~Filozof();
  bool jedz();
  void filozofuj();
};

#endif  //FILOZOF_H
