#ifndef SHELLSORT_H
#define SHELLSORT_H

#include "sorteermethode.h"
#include <iostream>

template <typename T>
class ShellSort : public Sorteermethode<T>{
    public:
        void operator()(vector<T> & v) const;
};

template <typename T>
void ShellSort<T>::operator()(vector<T> &v) const {
	for(int k = v.size()/2; k>0; k/=2) {
        for(int i = k; i<v.size(); i+=k) {
            int j = i-k;
            while(j >= 0 && v[j] > v[j+k]) {
                swap(v[j], v[j+k]);
                j-=k;
            }
        }
    }
}

#endif

