#include "Pot.h"
#include <iostream>

int Pot::nextId = 0;

Pot::Pot(int x) {
    diameter = x;
    id = nextId++;
}

// copy constructor
// this is to make sure that ids are unique
Pot::Pot(const Pot& pot) {
    if(this == &pot) {
        return;
    }

    diameter = pot.diameter;
    id = nextId++;
}

// copy operator
Pot& Pot::operator=(const Pot& pot) {
    diameter = pot.diameter;
    id = nextId++;

    return *this;
}

// move constructor
Pot::Pot(Pot&& pot) : diameter(pot.diameter), id(nextId++) {}

// move operator
Pot& Pot::operator=(Pot&& pot) {
    diameter = pot.diameter;
    id = pot.id;

    pot.diameter = 0;
    pot.id = -1;

    return *this;
}

// custom swap operation
void swap(Pot& pot1, Pot& pot2) {
    int diameter = pot1.diameter;
    int id = pot1. id;

    pot1.diameter = pot2.diameter;
    pot1.id = pot2.id;

    pot2.diameter = diameter;
    pot2.id = id;
}

int Pot::getId() {
    return id;
}

int Pot::getDiameter() {
    return diameter;
}
