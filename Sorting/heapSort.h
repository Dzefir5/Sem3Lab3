#ifndef HEAPSORT_H
#define HEAPSORT_H

#include "iSorter.h"
#include "../Sequences/Sequence.h"

template<typename T>
class HeapSorter : public ISorter<T>
{
    void heapHelper( Sequence<T>& seq, const int size, int index, std::function<int(const T&,const T&)> cmp) const
    {
        int parentIndex = index ;
        int leftChildIndex = 2 * parentIndex+ 1;
        int rightChildIndex = 2 * parentIndex + 2;

        if (leftChildIndex < size && cmp(seq[leftChildIndex], seq[parentIndex]) < 0) {
            parentIndex = leftChildIndex;
        }
        if (rightChildIndex < size && cmp(seq[rightChildIndex], seq[parentIndex]) < 0) {
            parentIndex = rightChildIndex;
        }
        if (parentIndex != index){
            my_swap(seq[index], seq[parentIndex]);
            heapHelper(seq, size, parentIndex, cmp);
        }

    }

public:
    Sequence<T>* Sort( Sequence<T>& seq, std::function<int(const T&,const T&)> cmp) const override {
        const int size = seq.GetLength();
        for (int i = size / 2 - 1; i >= 0; --i) {
            heapHelper(seq, size, i, cmp);
        }
        for (int i = size - 1; i > 0; --i) {
            std::swap(seq[0], seq[i]);
            heapHelper(seq, i, 0, cmp);
        }
        return &seq;
    }
};
#endif // HEAPSORT_H
