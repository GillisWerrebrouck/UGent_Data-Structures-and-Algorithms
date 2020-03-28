#ifndef MATCHER_H
#define MATCHER_H

#include <vector>
#include "Pot.h"
#include "Deksel.h"

class Matcher{
    private:
        Matcher() {}; // dissable the creation of object of this class ("static")
    public:
        static std::vector<std::pair <Deksel,Pot>> match(const std::vector<Deksel>&, const std::vector<Pot>&);
};

#endif
