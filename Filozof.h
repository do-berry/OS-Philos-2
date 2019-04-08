#ifndef FILOZOF_H
#define FILOZOF_H

#include <cstdlib>
#include <ctime>

class Filozof {
public:
  bool posilek;
  bool filozofowanie;
  Filozof();
  ~Filozof();
  void jedz();
  void filozofuj();
};

#endif  //FILOZOF_H