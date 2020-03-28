#ifndef DEKSEL_H
#define DEKSEL_H

#include "Pot.h"
#include "Comparator.h"

class Deksel{
    private:
        int id;
        int diameter;
        // this static property holds the next value for the unique id and is known in all instanced of this class
        static int nextId;
    public:
        Deksel(int x);
        Deksel(const Deksel& deksel);
        Deksel& operator=(const Deksel& deksel);
        Deksel(Deksel&& deksel);
        Deksel& operator=(Deksel&& deksel);

        friend void swap(Deksel& deksel1, Deksel& deksel2);

        int getId();
        // the only reason why there is a getter for diameter is to print and proof that correct pairs are matched
        // this is not used for comparison, comparison is achieved through the friend relation
        int getDiameter();
        // compare is a friend so that it can access the private properties of lid (deksel)
        friend int Comparator::compare(const Pot& pot, const Deksel& deksel);
};

#endif
