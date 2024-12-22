
#ifndef ISORTER_H
#define ISORTER_H
#pragma once


#include "../Sequences/Sequence.h"
#include <functional>


template<typename T>
class ISorter {
public:
    virtual Sequence<T>* Sort(Sequence<T>& sequence, std::function<int(const T&,const T&)> cmp) const = 0;
    virtual ~ISorter() = default;
};

#endif // ISORTER_H