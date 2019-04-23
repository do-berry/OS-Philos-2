#include "Filozof.h"
#include <curses.h>

Filozof::Filozof() {
  //licznik++;
  int start = std::rand() % 2;                   // posilek lub filozofowanie
	if (start == 0) {
		this->jedz();
	} else {
		this->filozofuj();
	}
}

Filozof::~Filozof() {}

bool Filozof::jedz() {
  if (!lewyWidelec) {
    //lewyWidelec = true;
    this->posilek = true;
    this->filozofowanie = false;
    return true;
  } else if (!prawyWidelec) {
    //prawyWidelec = true;
    this->posilek = true;
    this->filozofowanie = false;
    return true;
  } else {
    return false;
  }
}

void Filozof::filozofuj() {
	this->filozofowanie = true;
  this->posilek = false;
}

void Filozof::zmien(bool widelec, bool status) {
  if (widelec) {
    lewyWidelec = status;
  } else {
    prawyWidelec = status;
  }
}

bool Filozof::lewyWidelec = false;
bool Filozof::prawyWidelec = false;
int Filozof::licznik = -1;
