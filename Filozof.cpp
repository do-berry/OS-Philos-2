#include "Filozof.h"
#include <curses.h>

Filozof::Filozof() {
  licznik++;
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

bool Filozof::lewyWidelec = false;
bool Filozof::prawyWidelec = false;
int Filozof::licznik = -1;
