#include "Deksel.h"

int Deksel::nextId = 0;

Deksel::Deksel(int x) {
    diameter = x;
    id = nextId++;
}

// copy constructor
// this is to make sure that ids are unique
Deksel::Deksel(const Deksel& deksel) {
    if(this == &deksel) {
        return;
    }

    diameter = deksel.diameter;
    id = nextId++;
}

// copy operator
Deksel& Deksel::operator=(const Deksel& deksel) {
    diameter = deksel.diameter;
    id = nextId++;

    return *this;
}

// move constructor
Deksel::Deksel(Deksel&& deksel) : diameter(deksel.diameter), id(nextId++) {}

// move operator
Deksel& Deksel::operator=(Deksel&& deksel) {
    diameter = deksel.diameter;
    id = deksel.id;

    deksel.diameter = 0;
    deksel.id = -1;

    return *this;
}

// custom swap operation
void swap(Deksel& deksel1, Deksel& deksel2) {
    int diameter = deksel1.diameter;
    int id = deksel1. id;

    deksel1.diameter = deksel2.diameter;
    deksel1.id = deksel2.id;

    deksel2.diameter = diameter;
    deksel2.id = id;
}

int Deksel::getId() {
    return id;
}

int Deksel::getDiameter() {
    return diameter;
}
