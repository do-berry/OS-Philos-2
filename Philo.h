#ifndef PHILO_H
#define PHILO_H
#include <string>

using namespace std;

class Philo {
public:
  bool eat;
  bool think;
  Philo();
  ~Philo();
  void eating();
  string thinking();
};

#endif  //PHILO_H
