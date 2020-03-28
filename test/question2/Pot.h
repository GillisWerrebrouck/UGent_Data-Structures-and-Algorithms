#ifndef POT_H
#define POT_H

#include "Deksel.h"
#include "Comparator.h"

class Pot{
    private:
        int id;
        int diameter;
        // this static property holds the next value for the unique id and is known in all instanced of this class
        static int nextId;
    public:
        Pot(int x);
        Pot(const Pot& pot);
        Pot& operator=(const Pot& pot);
        Pot(Pot&& pot);
        Pot& operator=(Pot&& pot);

        friend void swap(Pot& pot1, Pot& pot2);

        int getId();
        // the only reason why there is a getter for diameter is to print and proof that correct pairs are matched
        // this is not used for comparison, comparison is achieved through the friend relation
        int getDiameter();
        // compare is a friend so that it can access the private properties of jar (pot)
        friend int Comparator::compare(const Pot& pot, const Deksel& deksel);
};

#endif
