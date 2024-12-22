#ifndef DEFAULTCOMPARATORS_H
#define DEFAULTCOMPARATORS_H
#pragma once
template<typename T>
int ascendingComparator(const T& a, const T& b) {
    if (a < b) {
        return 1;
    }
    if (a > b) {
        return -1;
    }
    return 0;
}

template<typename T>
int descendingComparator(const T& a, const T& b) {
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    return 0;
}
#endif // DEFAULTCOMPARATORS_H
