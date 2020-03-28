#ifndef COMPARATOR_H
#define COMPARATOR_H

class Pot;
class Deksel;

class Comparator{
    private:
        Comparator() {}; // dissable the creation of object of this class ("static")
    public:
        static int compare(const Pot& pot, const Deksel& deksel);
        static int compare(const Deksel& deksel, const Pot& pot);
};

#endif
