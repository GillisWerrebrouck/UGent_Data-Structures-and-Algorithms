#include "Comparator.h"
#include "Pot.h"
#include "Deksel.h"

/**
 * general rule:
 * 0: obj1 == obj2
 * 1: obj1 < obj2
 * -1: obj1 > obj2
**/ 

/**
 * 0: if jar (pot) is equal in size compared to lid (deksel)
 * 1: if jar (pot) is larger in size compared to lid (deksel)
 * -1: if jar (pot) is smaller in size compared to lid (deksel)
**/
int Comparator::compare(const Pot& pot, const Deksel& deksel) {
    if(pot.diameter == deksel.diameter) {
        return 0;
    } else if (pot.diameter < deksel.diameter) {
        return 1;
    } else {
        return -1;
    }
}

// the inverse of the method above since jar and lid are swapped in the argument list to comply to the general rule
int Comparator::compare(const Deksel& deksel, const Pot& pot) {
    return Comparator::compare(pot, deksel) * (-1);
}
