// testprogramma voor het social distance algoritme

#include "lijst.h"
#include <cstdlib>
#include <iostream>
#include <string>

Lijst<int> maakLijst(const std::vector<int>& v){
    Lijst<int> l;
    for(auto sleutel: v){
        l.voegToe(sleutel);
    }
    return l;
}

/**
 * applySocialDistancing algorithm:
 * The linked list is only iterated once which makes this algorithm have O(N) time complexity. Finding sublists, calculating overlap and adding isolation elements 
 * is all done in one iteration. This is achieved by keeping track of the min/max values of the previous and current sublist. Isolation elements are added in 
 * front of the currently iterated sublist (between the previous and current sublist), starting from sublist 2.
 * 
 * Additional explanation is provided at each step of the method (in lijst.h).
**/

int main() {
    {
        // we testen met lijsten van int sleutelwaarden
        std::vector<std::vector<int>> testvectoren;
        // bemerk dat volgorden in omgekeerde volgorde zijn gespecifeerd, omdat voegToe steeds aan het begin van de lijst toevoegt
        std::vector<int> v1 = {1,5,6,3,8,5,6,9,6,7,4,5,1,2};
        testvectoren.push_back(v1);
        std::vector<int> v2 = {7,5,3,2,7,4,5,1,5,6,7,9,1};
        testvectoren.push_back(v2);
		std::vector<int> v3 = {20,1,1,1,1,1,1,6,6,6,6,6,6,6,10};
        testvectoren.push_back(v3);
		std::vector<int> v4 = {1,2,3,4,5,6,7,1,2,3,4,5,6,1,2,3,4,5,1,2,3};
        testvectoren.push_back(v4);
		std::vector<int> v5 = {3,2,1,4,3,2,1,5,4,3,2,1,6,5,4,3,2,1};
        testvectoren.push_back(v5);
        std::vector<int> v6 = {1,5,9,13,13,12,8,5,1};
        testvectoren.push_back(v6);
        
        int socialDistance = 3;
        int isolationKey = 0;

        for(std::vector<int>& v: testvectoren){
            Lijst<int> l;
            // eerste element in vector wordt laatste element van gelinkte lijst
            l = maakLijst(v);
            std::cerr << "Testen van de vector: " << std::endl;
            l.schrijf(std::cerr);
            std::cerr << std::endl;
            l.applySocialDistancing(socialDistance, isolationKey);
            std::cerr << "Na toepassen van social distancing: " << std::endl;
            l.schrijf(std::cerr);
            std::cerr << std::endl;
        }
    }

    std::cerr << "OK\n";

    return 0;
}
