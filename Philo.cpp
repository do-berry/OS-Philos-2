#include "Philo.h"
#include <cstdlib>
#include <string>

Philo::Philo() {
  int startWith = rand() % 2;
	if (startWith == 0) {
		this->eating();
	} else {
		this->thinking();
	}
}

Philo::~Philo() {}

void Philo::eating() {
  this->eat = true;
  this->think = false;
}

string Philo::thinking() {
	return "thinks";
}
