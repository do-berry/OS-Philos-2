#include "Filozof.h"

Filozof::Filozof() {
  int start = std::rand() % 2;                   // posilek lub filozofowanie
	if (start == 0) {
		this->jedz();
	} else {
		this->filozofuj();
	}
}

Filozof::~Filozof() {}

void Filozof::jedz() {
	this->posilek = true;
  this->filozofowanie = false;
}

void Filozof::filozofuj() {
	this->filozofowanie = true;
  this->posilek = false;
}
