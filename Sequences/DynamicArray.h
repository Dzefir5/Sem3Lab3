#pragma once
#include "my_swap.h"
#include <stdexcept>



template <typename T>
class dynamicArray{

private:
    int size=0;
    T* elements=nullptr;
    void swap(dynamicArray<T>& toSwap){
        my_swap(size,toSwap.size);
        my_swap(elements,toSwap.elements);
    }
public:
    //Constructors
    dynamicArray():size(0),elements(nullptr){} ;
    dynamicArray(int size):size(size),elements(nullptr){
        if(size<0) 
            throw std::invalid_argument("invalid argument in constructor");
        if(size>0) 
            elements=new T[size];
    }
    dynamicArray( T* items, int count):dynamicArray<T>(count){
        if(items==nullptr) 
            throw std::invalid_argument("invalid argument in constructor");
        for(int i=0;i<count;i++){
            elements[i]=items[i];
        }
    }
    dynamicArray( const T& fillElem, int count):dynamicArray<T>(count){
        for(int i=0;i<size;i++){
            elements[i]=fillElem;
        }
    }
    dynamicArray(const dynamicArray<T>& array):dynamicArray<T>(array.elements,array.size){}

    //Functions
    T Get(int index) const {
        if(size<=index||index<0)
            throw std::out_of_range("Invalid index get");
        return elements[index];
    }

    int GetLength() const { return this->size; }

    void Set(const T& value,int index) {
        if(size<=index||index<0) 
            throw std::out_of_range("Invalid index set"+std::to_string(size<=index)+"|"+std::to_string(index<0));
        elements[index]=value;
    }
    void Resize(int newSize,int offset=0){
        if(newSize<0||offset<0||offset+size>newSize)
            throw std::invalid_argument("Invalid argument in Resize fucntion");
        if(newSize==0){
            delete[] elements;
            elements=nullptr;
            return;
        }
        T* buf = new T[newSize];
        if(elements!=nullptr){
            for(int i=0;i<std::min(size,newSize);i++){
                buf[i+offset]=elements[i];
            }
            delete[] elements;
        }
        elements=buf;
        size=newSize;
    }

    T& operator[](int index){
        if(size<=index)
            throw std::out_of_range("Invalid index");
        if(size<=0) 
            throw std::out_of_range("Getter call in empty collection");
        return elements[index];
    }
    const T& operator[](int index) const{
        if(size<=index)
            throw std::out_of_range("Invalid index");
        if(size<=0) 
            throw std::out_of_range("Getter call in empty collection");
        return elements[index];
    }
    bool operator==(const dynamicArray<T>& array){
        if(array.size!=size) 
            return false;
        for(int i =0 ; i<array.size;i++){
            if(array[i]!=(*this)[i]) return false;
        }
        return true;
    }
    dynamicArray<T>& operator=( dynamicArray<T> array){ ///COPY AND SWAP IDIOM
       swap(array);
       return *this;
    }
    
    //Destructor
    virtual ~dynamicArray(){
        delete[] elements;
    }
};