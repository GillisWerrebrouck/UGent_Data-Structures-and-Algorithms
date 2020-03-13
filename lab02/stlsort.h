#ifndef STLSORT_H
#define STLSORT_H

#include "sorteermethode.h"
#include <algorithm>

template <typename T>
class STLSort : public Sorteermethode<T>{
    public:
        void operator()(vector<T> & v) const;
};

template <typename T>
void STLSort<T>::operator()(vector<T> & v) const{
    sort(v.begin(), v.end());
}

#endif

