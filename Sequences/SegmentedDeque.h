#pragma once

#include "Sequence.h"
#include "DynamicArray.h"
#include "my_swap.h"
#include "my_abs.h"
#include <iostream>


template <typename T>
class SegmentedDeque : public Sequence<T> {
public:
    dynamicArray<T*>* data = nullptr;
    int chankSize = 1024;
    int chankCount = 0;
    int size=0;
    int start=0;
    int end =0;


protected:
    void swap(SegmentedDeque<T>& toSwap){
        my_swap( data,toSwap.data);
        my_swap(chankCount,toSwap.chankCount);
        my_swap( size,toSwap.size);
        my_swap(start,toSwap.start);
        my_swap(end,toSwap.end);
    }
    virtual SegmentedDeque<T>* Instance() = 0;
    virtual const SegmentedDeque<T>* Instance() const = 0;
    
public:

    SegmentedDeque(){
        data = new dynamicArray<T*>(1);
        (*data)[0] = new T[chankSize];
        chankCount =1 ;
        size = 0;
        start = chankSize/2;
        end = (chankSize/2)-1;
    }
    SegmentedDeque(int inSize):size(inSize){
        int startOffset = ( inSize % chankSize )/2;
        chankCount = inSize/chankSize + 1;
        data = new dynamicArray<T*>(chankCount);
        for(int i =0 ;i<chankCount ; i++){
            (*data)[i] = new T[chankSize];
        }
        if(chankCount ==1){
            start = (chankSize - inSize)/2;
            end = start + inSize - 1;
        }
        if(chankCount >=2){
            startOffset = inSize/2;
            start = chankSize - startOffset ;
            end = inSize - startOffset - 1 ;
        }
        /*
        if(chankCount >= 3){
            startOffset  = (inSize - (chankCount-2)*chankSize)/2;
            start = chankSize - startOffset ;
            end = inSize - (chankCount-2)*chankSize - startOffset -1 ;
        }
        */
    }   
    SegmentedDeque(const T& fillElem , int inSize):SegmentedDeque(inSize){
        for(int i =0;i<inSize;i++){
            (*this)[i] = fillElem;
        }
    }   
    SegmentedDeque(T* items, int inSize):SegmentedDeque(inSize){
        for(int i =0;i<inSize;i++){
            //this->Set(items[i],i);
            (*this)[i] = items[i];
        }
    }
    SegmentedDeque(const Sequence<T>& seq):SegmentedDeque(seq.GetLength()){
        for(int i =0;i<seq.GetLength();i++){
            (*this)[i] = seq.Get(i);
        }
    }
    SegmentedDeque(const SegmentedDeque<T>& deq):SegmentedDeque(deq.GetLength()){
        for(int i =0;i<deq.GetLength();i++){
            (*this)[i] = deq.Get(i);
        }
    }


    T Get(int index)  const override{
        if(index<0||index>=size) 
            throw std::out_of_range("invalid index");
        if(index < chankSize - start){
            return (*data)[0][index+start] ;
        }
        if(index>= size - end - 1 ){
            return (*data)[chankCount-1][index - (chankSize - start) - (chankCount -2)*chankSize] ; 
        }
        int chankIndex = ( index - ( chankSize - start )  )/ chankSize + 1 ;
        return (*data)[ chankIndex  ][ ( index - (chankSize - start) ) % chankSize] ; 
    }
    T GetFirst() const override {
        if(size==0) 
            throw std::out_of_range("invalid index");
        return (*data)[0][start];
    }
    T GetLast() const override {
        if(size==0) 
            throw std::out_of_range("invalid index");
        return (*data)[chankCount-1][end];
    }
    SegmentedDeque<T>* Set ( int index, const T& item ) override {
        SegmentedDeque<T>* result = Instance();
        if(index<0||index>=result->size) 
            throw std::out_of_range("invalid index");
        if(index < result->chankSize - result->start){
            (*(result->data))[0][index+ result->start] = item ; 
            return result;
        }
        if(index>= result->size - result->end - 1 ){
            (*(result->data))[result->chankCount-1][index - (result->chankSize - result->start) - (result->chankCount -2)*result->chankSize] = item ; 
            return result ; 
        }
        
        int chankIndex = ( index - ( result->chankSize - result->start )  )/ result->chankSize + 1 ;
        (*(result->data))[ chankIndex  ][ ( index - (result->chankSize - result->start) ) % result->chankSize] = item ;
        return result ; 
    }
    int GetLength() const override{
        return size;
    }

    SegmentedDeque<T>* Append (const T& item) override {
        SegmentedDeque<T>* result = Instance();
        if(result->end < chankSize - 1){
            result->end++;
            result->size++;
            (*(result->data))[result->chankCount-1][result->end]=item;
        }else{
            (result->data)->Resize(chankCount+1);
            (*(result->data))[result->chankCount]= new T[chankSize];
            result->end=0;
            result->chankCount++;
            result->size++;
            (*(result->data))[result->chankCount-1][result->end]=item;
        }

        return result;
    }
    SegmentedDeque<T>* Prepend(const T& item) override {
        SegmentedDeque<T>* result = Instance();
        if(result->start > 0 ){
            result->start--;
            result->size++;
            (*(result->data))[0][result->start]=item;
        }else{
            result->data->Resize(result->chankCount+1,1);
            (*(result->data))[0]= new T[chankSize];
            result->start = chankSize-1;
            result->chankCount++;
            result->size++;
            (*(result->data))[0][result->start]=item;
        }

        return result;
    }

    SegmentedDeque<T>* PopFront()  {
        SegmentedDeque<T>* result = Instance();
        if(result->size<0) 
            throw std::invalid_argument("invalid argument in popfront");
        result->size--;
        if (start==chankSize-1){
            result->data->Resize(result->data->GetLength()-1,-1);
            result->chankCount--;
            result->start = 0;
            return result;
        }
        result->start++;
        return result;
    }
    SegmentedDeque<T>* PopBack()  {
        SegmentedDeque<T>* result = Instance();
        if(result->size<0) 
            throw std::invalid_argument("invalid argument in popback");
        result->size--;
        if (result->end==0){
            std::cout<<"call resize"<<std::endl;
            result->data->Resize(result->data->GetLength()-1,0);
            result->chankCount--;
            result->end = chankSize-1;
            return result;
        }
        result->end--;
        return result;
    }

    SegmentedDeque<T>* RemoveAt(int index) override {
        SegmentedDeque<T>* result = Instance();
        if(index == 0){
            result->PopFront();
            return result;
        }
        if(index == size-1){
            result->PopBack();
            return result;
        }
        for(int i = index ; i<result->size-1;i++){
            (*result)[i]=(*result)[i+1];
        }
        result->PopBack();
        
        return result;
    }
    SegmentedDeque<T>* InsertAt(const T& item , int index) override {
        SegmentedDeque<T>* result = Instance();
        if(index == 0){
            result->Prepend(item);
            return result;
        }
        if(index == size){
            result->Append(item);
            return result;
        }
        result->Append(item);
        for(int i = result->size-1 ; i>index;i--){
            (*result)[i]=(*result)[i-1];
        }
        (*result)[index] = item;
        

        return result;
    }

    bool IsEmpty(){
        return size==0;
    }

    T& operator[](int index) override {
        if(index<0||index>=size) 
            throw std::out_of_range("invalid index");
        if(index < chankSize - start){
            return (*data)[0][index+start] ;
        }
        if(index>= size - end - 1 ){
            return (*data)[chankCount-1][index - (chankSize - start) - (chankCount -2)*chankSize] ; 
        }
        int chankIndex = ( index - ( chankSize - start )  )/ chankSize + 1 ;
        return (*data)[ chankIndex  ][ ( index - (chankSize - start) ) % chankSize] ; 
    }

    virtual ~SegmentedDeque(){
        for(int i =0 ;i<data->GetLength();i++){
            delete[] data->Get(i);
        }
        delete data;
    }
};

template <typename T>
void PrintDeque(const SegmentedDeque<T>& deq) {
        std::cout<<"------------------"<<std::endl;
        std::cout<<"Start index : "<<deq.start<<std::endl<<"End index : "<<deq.end<<std::endl;
        std::cout<<"------------------"<<std::endl;
        std::cout<<"Chank " << 1 <<" : ";
        if(deq.chankCount ==1){
            for(int i = 0 ; i < deq.chankSize; i ++){
                if(i<deq.start||i>deq.end){
                    std::cout<<"[_]"<<" ";
                }else{
                    std::cout<<"["<<(*(deq.data))[0][i]<<"] ";
                }
            }
            std::cout<<std::endl;
            std::cout<<"------------------"<<std::endl;
            return;
        }
        for(int i = 0 ; i<deq.chankSize; i ++){
            if(i<deq.start){
                std::cout<<"[_]"<<" ";
            }else{
                std::cout<<"["<<(*(deq.data))[0][i]<<"] ";
            }
        }
        std::cout<<std::endl;
        for(int i  = 0 ; i<deq.chankCount-2;i++){
            std::cout<<"Chank " << i+2 <<" : ";
            for(int j = 0 ;j<deq.chankSize;j++){
                std::cout<<"["<<(*(deq.data))[i+1][j]<<"] ";
            }
            std::cout<<std::endl;
        }
        std::cout<<"Chank " << deq.chankCount <<" : ";
        for(int i = 0 ; i<deq.chankSize; i ++){
            if(i>deq.end){
                std::cout<<"[_]"<<" ";
            }else{
                std::cout<<"["<<(*(deq.data))[deq.chankCount-1][i]<<"] ";
            }
        }
        std::cout<<std::endl;
        std::cout<<"------------------"<<std::endl;
        std::cout<<"Deque size  : "<<deq.size<<std::endl;
        std::cout<<"Chank count : "<<deq.chankCount<<std::endl;
        std::cout<<"Chank size  : "<<deq.chankSize<<std::endl;
        std::cout<<"Deque capacity : "<<deq.chankCount*deq.chankSize<<std::endl;
        std::cout<<"------------------"<<std::endl;
        return;
    }

template <typename T>
class MutableSegmentedDeque : public SegmentedDeque<T> {
private:
    SegmentedDeque<T>* Instance() override {
        return static_cast< SegmentedDeque<T>* >(this);
    }
    
    const SegmentedDeque<T>* Instance() const  override {
        return static_cast<const SegmentedDeque<T>*>(this);
    }

public:
    using SegmentedDeque<T>::SegmentedDeque;

    MutableSegmentedDeque<T>* Concat( const Sequence <T>& seq) const override {
        MutableSegmentedDeque<T>* result = new MutableSegmentedDeque<T>(*this);
        for(int i=0;i<seq.GetLength();i++){
            result->Append(seq.Get(i));
        }
        return result;
    } 
    MutableSegmentedDeque<T>*  GetSubsequence(int startIndex, int endIndex) const override{
        if(startIndex<0||endIndex<startIndex) throw std::invalid_argument("");
        if(endIndex>=this->size) throw std::out_of_range("");
        MutableSegmentedDeque<T>* result = new MutableSegmentedDeque<T>(endIndex - startIndex + 1);
        for(int i = 0 ; i < endIndex-startIndex +1;i++){
            result->Set(i,this->Get(startIndex+i));
        }
        return result;
    }

    MutableSegmentedDeque<T>* Slice(int index, int offset , const Sequence<T> &seq ) override {
       MutableSegmentedDeque<T>* result = new MutableSegmentedDeque<T>(*this);
        
        if( my_abs(index)>result->GetLength()) throw std::invalid_argument("");
        int start =0 ;
        if(index<0) { start =result->GetLength() + index;  } else { start = index; }
        int i =start;
        for( ; i< std::min(result->GetLength() ,  start+seq.GetLength() );i++) {
            result->Set(i,seq.Get(i-start));
        }   
        int removeCount =0;
        
        for( ;(i< std::min(result->GetLength() , start+offset ) ) && ( removeCount<offset-seq.GetLength() );removeCount++){
            result->RemoveAt(i);
        }
        return result;
    }
     MutableSegmentedDeque<   MutableSegmentedDeque<T>* >* Split( bool (*func)(T input) ) const  {
        MutableSegmentedDeque<  MutableSegmentedDeque<T>* >* result = new MutableSegmentedDeque<  MutableSegmentedDeque<T>* >();
        MutableSegmentedDeque<T>* buf = new MutableSegmentedDeque<T>();
        for(int i = 0 ; i<this->GetLength();i++){
            buf->Append(this->Get(i));
            if( (*func)(this->Get(i)) == true){
                result->Append(buf);
                if(i!=this->GetLength()-1){
                    buf = new MutableSegmentedDeque<T>();  
                }
                
            } 
        }
        if(buf->GetLength()!=0){
            result->Append(buf);  
        }
        return result;
    }
    MutableSegmentedDeque<T>& operator=(const Sequence<T> &seq){
        MutableSegmentedDeque<T> result (seq);
        swap(*this,result);
        return *this;
    }

    MutableSegmentedDeque<T>& operator+=(const Sequence<T> &seq){
        for(int i=0;i<seq.GetLength();i++){
            this->Append(seq.Get(i));
        }
        return *this;
    }
};
template <typename T>
MutableSegmentedDeque<T>& operator+(const Sequence<T> &seq1,const Sequence<T> &seq2){
    MutableSegmentedDeque<T>* result = new  MutableSegmentedDeque<T>(seq1);
    (*result)+=seq2;
    return *result;
}


template <typename T>
class ImmutableSegmentedDeque : public SegmentedDeque<T> {
private:
    ImmutableSegmentedDeque<T>* Clone(){
        return new ImmutableSegmentedDeque<T>(*this);
    }
    ImmutableSegmentedDeque<T>* Clone() const{
        return new ImmutableSegmentedDeque<T>(*this);
    }
    SegmentedDeque<T>* Instance() override {
        return static_cast< SegmentedDeque<T>* >( Clone() );
    }
    
    const SegmentedDeque<T>* Instance() const  override {
        return static_cast<const SegmentedDeque<T>*>( Clone() );
    }

public:
    using SegmentedDeque<T>::SegmentedDeque;

    ImmutableSegmentedDeque<T>* Concat( const Sequence <T>& seq) const override {
        MutableSegmentedDeque<T>* result = new MutableSegmentedDeque<T>(this->GetLength() + seq.GetLength());
        for(int i=0;i<this->GetLength();i++){
            result->Set(i,this->Get(i));
        }
        for(int i=0;i<seq.GetLength();i++){
            result->Set(i+this->GetLength(),seq.Get(i));
        }
        ImmutableSegmentedDeque<T>* res = new ImmutableSegmentedDeque<T>(*result);
        delete result;
        return res;
    } 
    ImmutableSegmentedDeque<T>*  GetSubsequence(int startIndex, int endIndex) const override{
        if(startIndex<0||endIndex<startIndex) throw std::invalid_argument("");
        if(endIndex>=this->size) throw std::out_of_range("");
        MutableSegmentedDeque<T>* result = new MutableSegmentedDeque<T>(endIndex - startIndex + 1);
        for(int i = 0 ; i < endIndex-startIndex +1;i++){
            result->Set(i,this->Get(startIndex+i));
        }
        ImmutableSegmentedDeque<T>* res = new ImmutableSegmentedDeque<T>(*result);
        delete result;
        return res;
    }

    ImmutableSegmentedDeque<T>* Slice(int index, int offset , const Sequence<T> &seq ) override {
        MutableSegmentedDeque<T>* result = new MutableSegmentedDeque<T>(*this);
        if( my_abs(index)>result->GetLength()) throw std::invalid_argument("");
        int start =0 ;
        if(index<0) { start =result->GetLength() + index;  } else { start = index; }
        int i =start;
        for( ; i< std::min(result->GetLength() ,  start+seq.GetLength() );i++) {
            result->Set(i,seq.Get(i-start));
        }   
        int removeCount =0;
        
        for( ;(i< std::min(result->GetLength() , start+offset ) ) && ( removeCount<offset-seq.GetLength() );removeCount++){
            result->RemoveAt(i);
        }
        ImmutableSegmentedDeque<T>* res = new ImmutableSegmentedDeque<T>(*result);
        delete result;
        return res;
    }

    Sequence<  Sequence<T>* >* Split( bool (*func)(T input) ) const  {
        MutableSegmentedDeque< Sequence<T>* >* result = new MutableSegmentedDeque< Sequence<T>* >();
        MutableSegmentedDeque<T>* buf = new MutableSegmentedDeque<T>();
        for(int i = 0 ; i<this->GetLength();i++){
            buf->Append(this->Get(i));
            if( (*func)(this->Get(i)) == true){
                result->Append(buf);
                buf = new MutableSegmentedDeque<T>();
            } 
        }
        result->Append(buf);
        ImmutableSegmentedDeque< Sequence<T>* >* finalresult = new ImmutableSegmentedDeque< Sequence<T>* >(*result);
        delete result;
        return finalresult;
    }

    ImmutableSegmentedDeque<T>& operator=(const Sequence<T> &seq){
        ImmutableSegmentedDeque<T> result (seq);
        swap(*this,result);
        return *this;
    }

};