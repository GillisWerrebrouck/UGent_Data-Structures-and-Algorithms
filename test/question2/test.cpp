#include "Deksel.cpp"
#include "Pot.cpp"
#include "Comparator.cpp"
#include "Matcher.cpp"
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

void vulVectoren(const std::vector<int>& diameters, std::vector<Deksel>& deksels, std::vector<Pot>& potjes){
    for(const int& x : diameters){
        deksels.push_back(Deksel(x));
        potjes.push_back(Pot(x));
    }

    static std::mt19937 eng{std::random_device{}()};    
    std::shuffle(deksels.begin(),deksels.end(),eng);
    std::shuffle(potjes.begin(),potjes.end(),eng);
}

// print an individual pair
void printPair(std::pair <Deksel,Pot> & pair) {
    std::cout << "ID: " << pair.first.getId() << ", DIAMETER: " << pair.first.getDiameter() << " - " 
                << "ID: " << pair.second.getId() <<  ", DIAMETER: " << pair.second.getDiameter() << std::endl;
}

// print all pairs in a clear format
void printPairs(std::vector<std::pair <Deksel,Pot>> & pairs) {
    for(int i = 0; i < pairs.size(); i++) {
        printPair(pairs[i]);
    }
    std::cout << std::endl;
}

/**
 * match algorithm:
 * The lids (deksels) and jars (potjes) vectors are first copied into one vector of pairs, each pair holds a lid and a jar. 
 * The pairs aren't matched yet but the vectors are copied into one vector of pairs immediately instead of making a copy of both vectors. 
 * The reason for this is because it is better in terms of performance. If both vectors would be copied to two other vectors that would be 2 copy operations. 
 * At the end of the algorithm those two vectors would be copied to a vector of pairs, this would be a third copy operation which is very expensive and not 
 * good in terms of performance. 
 * The thing that could be considered a code smell is the slightly duplicate partition methods, this is because of the ".first" and ".second" parts in each method. 
 * This could be solved by using if-statements but this wouldn't be clear code and it would require extra if-checks. The performance achieved by only copying once 
 * instead of three times is worth this slight code smell.
 * 
 * The sorting/matching of the pairs is then done by sorting the vector of pairs in place without any extra copies. First the left value is used as a pivot to 
 * partition the jars and this method returns the index of this pivot after partitioning. It is certain that the element on this index is already in the correct 
 * possition. This pivot is then used to partition the lids. After all this, the function is recursively called, first with the left part of the pivot and next 
 * with the second part of the pivot. The recursion stops when all elements are matched (l < r).
 * 
 * Additional explanation is provided at each step of the methods and classes.
**/

int main(){
    std::vector<int> diameters {1, 1, 2, 3, 4, 4, 4, 5, 6, 6, 7, 8, 8, 9, 9, 10};
    std::vector<Deksel> deksels;
    std::vector<Pot> potjes;
    vulVectoren(diameters, deksels, potjes);

    std::vector<std::pair <Deksel,Pot>> pairs = Matcher::match(deksels, potjes);
    printPairs(pairs);

    std::cout << "OK" << std::endl;
    return 0;
}