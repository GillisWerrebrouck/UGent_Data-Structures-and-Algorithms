#include "Matcher.h"
#include "Comparator.h"

using std::vector;
using std::pair;

void matchPairs(vector<pair<Deksel,Pot>> & pairs, int l, int r);
int partitionDeksels(vector<pair<Deksel,Pot>> & pairs, int l, int r, int pivot);
int partitionPotjes(vector<pair<Deksel,Pot>> & pairs, int l, int r, int pivot);

vector<pair<Deksel,Pot>> Matcher::match(const vector<Deksel>& deksels, const vector<Pot>& potjes) {
    // copy both vectors in one vector of pairs
    vector<pair<Deksel,Pot>> pairs;
    for(int i = 0; i < deksels.size(); i++) {
        pairs.push_back(std::make_pair(deksels[i], potjes[i]));
    }

    // match all pairs by using a modified version of the (single pivot) quicksort algorithm
    matchPairs(pairs, 0, pairs.size()-1);

    return pairs;
}

// this method is recursively called on the left and right side of the pivot until all pairs are matched
void matchPairs(vector<pair<Deksel,Pot>> & pairs, int l, int r) {
    if(l < r) {
        // the left element in the current partition is used as the pivot to partition the jars
        // the return value is the index of this left element after partitioning and will be used 
        // as pivot to partition the lids
        int pivot = partitionPotjes(pairs, l, r, l);
        partitionDeksels(pairs, l, r, pivot);

        // recursively call this method with the part before the pivot and after the pivot
        matchPairs(pairs, l, pivot-1);
        matchPairs(pairs, pivot+1, r);
    }
}

// this method is used to partition the jars in the vector of pairs<lids, jars>
int partitionPotjes(vector<pair<Deksel,Pot>> & pairs, int l, int r, int pivot) {
    const Deksel & pivotDeksel = pairs[pivot].first;
    int i = l;
    /**
     * partition all elements by swapping elements smaller than the pivot in front of the pivot and elements larger than the pivot after it
     * 
     * at the same time l and r are adjusted as needed;
     *   - if r is already correct then it shouldn't be checked again (i <= r), because of this r is decreased when needed (else-if-clause)
     *   - l is increased when an element smaller than the pivot is moved in front of it (if-clause)
     *   - i is increased when it has already checked the associated element, this is the case when an element smaller than 
     *     the pivot is moved in front of the pivot (if-clause) and when an element equal to the pivot is left as is (else-clause)
    **/ 
    while(i <= r) {
        if(Comparator::compare(pairs[i].second, pivotDeksel) == 1) {
            swap(pairs[i++].second, pairs[l++].second);
        } else if (Comparator::compare(pairs[i].second, pivotDeksel) == -1) {
            swap(pairs[i].second, pairs[r--].second);
        } else {
            i++;
        }
    }
    // l is the index of the pivot element after partitioning
    return l;
}

/** 
 * this method is used to partition the lids in the vector of pairs<lids, jars>
 * 
 * the method below is the same as partitionPotjes but couldn't be merged into one because 
 * of the use of .first and .second to access the correct objects in the pairs
**/
int partitionDeksels(vector<pair<Deksel,Pot>> & pairs, int l, int r, int pivot) {
    const Pot & pivotPot = pairs[pivot].second;
    int i = l;

    while(i <= r) {
        if(Comparator::compare(pairs[i].first, pivotPot) == 1) {
            swap(pairs[i++].first, pairs[l++].first);
        } else if (Comparator::compare(pairs[i].first, pivotPot) == -1) {
            swap(pairs[i].first, pairs[r--].first);
        } else {
            i++;
        }
    }
    return l;
}
