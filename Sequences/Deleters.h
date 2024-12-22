#pragma once
#include <cstddef>

template<typename T>
struct My_Universal_Deleter{
    void operator()(T* ptr){
        delete ptr;
    }
};
template<typename T>
struct My_Universal_Deleter<T[]>{
    void operator()(T* ptr){
        delete[] ptr;
    }
};
template<typename T,size_t N>
struct My_Universal_Deleter<T[N]>{
    void operator()(T* ptr){
        delete[] ptr;
    }
};


/* //для тестирования
template<>
struct My_Universal_Deleter<int[]>{
    void operator()(int* ptr){
        (*ptr)+=10;
    }
};
*/


