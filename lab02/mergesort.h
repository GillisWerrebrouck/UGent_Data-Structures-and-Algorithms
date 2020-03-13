#ifndef MERGESORT_H
#define MERGESORT_H

#include "sorteermethode.h"

template <typename T>
class MergeSort : public Sorteermethode<T>{
    public:
        void operator()(vector<T> & v) const;
        void merge(vector<T> &v, vector<T> &temp, int l, int m, int r) const;
};

int min(int x, int y) {
    return x < y ? x : y;
}

template <typename T>
void MergeSort<T>::operator()(vector<T> & v) const{
    vector<T> temp(v.size()/2);

    for (int curr_size = 1; curr_size < v.size()-1; curr_size *= 2) {
        for(int l = 0; l < v.size()-1; l += curr_size) {
            int m = min(l + curr_size-1, v.size()-1);
            int r = min(l + 2*curr_size-1, v.size()-1);

            merge(v, temp, l, m, r);
        }
    }
}

template <typename T>
void MergeSort<T>::merge(vector<T> &v, vector<T> &temp, int l, int m, int r) const {
    int p = l;
	while(p <= m) {
        swap(temp[p-l], v[p]);
        p++;
    }

    p = 0;
    int q = m+1;
    int k = l;

    while(p <= m-l && q <= r) {
        if(temp[p] <= v[q]) {
            swap(temp[p++], v[k++]);
        } else {
            swap(v[q++], v[k++]);
        }
    }

    while(p <= m-l) {
        swap(temp[p++], v[k++]);
    }

    while(q <= r) {
        swap(v[q++], v[k++]);
    }
}

#endif

