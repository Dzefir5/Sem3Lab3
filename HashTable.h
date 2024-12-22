#ifndef LAB3_HASH_TABLE_H
#define LAB3_HASH_TABLE_H

#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>

const double loadFactor = 0.75;

template<typename T>
size_t hash(const T &key) {
    return std::hash<T>{}(key);
}

template<class T1, class T2>
class HashTable {
public:
    class Node {
    private:
        bool is_taken = false; //занята ли нода
        bool is_last = false; //является ли узел последним в цепочке коллизий
        size_t prev;
        size_t next;
    public:
        T1 key;
        T2 value;
        Node() = default;
        Node(const T1 &key, const T2 &value, bool is_taken, bool is_last, size_t prev, size_t next) : key(key), value(value), is_taken(is_taken), is_last(is_last), prev(prev), next(next) {}
        
        ~Node() = default;

        bool isTaken() const {
            return is_taken;
        }

        bool isLast() const {
            return is_last;
        }

        size_t getPrev() const {
            return prev;
        }

        size_t getNext() const {
            return next;
        }
        
        void setTaken() {
            is_taken = true;
        }

        void setNotTaken() {
            is_taken = false;
        }

        void setLast() {
            is_last = true;
        }

        void setNotLast() {
            is_last = false;
        }

        void setNext(size_t next) {
            this->next = next;
        }

        void setPrev(size_t prev) {
            this->prev = prev;
        }
    };

    class Iterator {
    private:
        Node *ptr = nullptr;
    public:
        virtual Node &operator*() const {
            return *ptr;
        }
        virtual bool operator==(const Iterator &other) const {
            return ptr == other.ptr;
        }
        virtual bool operator!=(const Iterator &other) const {
            return ptr != other.ptr;
        }
        virtual Iterator &operator++() {
            if ((*ptr).isLast()) {
                ++ptr;
                return *this;
            }
            ++ptr;
            while (!(*ptr).isTaken() && !(*ptr).isLast()) {
                ++ptr;
            }
            return *this;
        }

        virtual ~Iterator() = default;

        Iterator(Node *init_ptr) : ptr(init_ptr) {}
    };
private:
    Node *table = nullptr;
    size_t capacity = 0;
    size_t size = 0;
public:
    HashTable() = default;
    HashTable(size_t capacity) {
        table = new Node[capacity];
        this->capacity = capacity;
        for (size_t i = 0; i < capacity; ++i) {
            table[i].setNext(i);
            table[i].setPrev(i);
        }
    }

    HashTable(HashTable &other) {
        table = new Node[other.capacity];
        capacity = other.capacity;
        for (auto &i : other) {
            insert(i.key, i.value);
        }
        size = other.getSize();
    }

    HashTable(HashTable &&other) : table(other.table), capacity(other.capacity), size(other.size) {
        other.table = nullptr;
        other.size = 0;
        other.capacity = 0;
    }
    void resize() {
        auto old_capacity = capacity;
        if (capacity == 0)
            capacity = 1;
        else capacity *= 2;
        Node *new_table = new Node[capacity];
        auto *old_table = table;
        table = new_table;
        size = 0;
        for (size_t i = 0; i < old_capacity; ++i) {
            if (old_table[i].isTaken()) {
                insert(old_table[i].key, old_table[i].value);
            }
        }
        for (size_t i = 0; i < capacity; ++i) {
            if (!table[i].isTaken()) {
                table[i].setNext(i);
                table[i].setPrev(i);
            }
        }
        table[capacity - 1].setLast();
        delete[] old_table;
    }
    virtual T2 &operator[](const T1 &key) {
        auto result = find(key);
        if (result == end()) {
            throw std::out_of_range("Key not found");
        }
        return (*result).value;
    }

    HashTable &operator=(const HashTable &other) {
        clear();
        table = new Node[other.capacity];
        capacity = other.capacity;
        size = 0;
        auto other_not_const = const_cast<HashTable &>(other);
        for (auto &i : other_not_const) {
            insert(i.key, i.value);
        }
        return *this;
    }

    HashTable &operator=(HashTable &&other) {
        if (this != &other) {
            clear();
            table = other.table;
            capacity = other.capacity;
            size = other.size;
            other.table = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    virtual Iterator begin() {
        for (size_t i = 0; i < capacity; ++i) {
            if (table[i].isTaken()) {
                return Iterator(table + i);
            }
        }
        return end();
    }

    virtual Iterator end() {
        return Iterator(table + capacity);
    }

    virtual Iterator find(const T1 &key) {
        if (size == 0) {
            return end();
        }
        auto index = hash(key) % capacity;
        while (table[index].key != key || !table[index].isTaken()) {
            index = table[index].getNext();
            if (index == (hash(key) % capacity)) {
                return end();
            }
        }
        return Iterator(table + index);
    }

    virtual void insert(const T1 &key, const T2 &value) {
        if (size >= loadFactor*capacity) {
            std::cout<<"Capacity exceeded "<<size<<"_"<<capacity<<std::endl;
            resize();
        }
        auto index = hash(key) % capacity;
        auto hash_index = index;
        auto prev_index = table[hash_index].getPrev();
        auto next_index = hash_index;
        while (table[index].isTaken()) {
            index = (index + 1) % capacity;
        }
        if (index == hash_index) {
            prev_index = index;
        }
        auto is_last = (index == getCapacity() - 1) ? true : false;
        table[index] = Node(key, value, true, is_last, prev_index, next_index);
        if (table[hash_index].getPrev() != hash_index) {
            auto pre_hash_index = table[hash_index].getPrev();
            table[pre_hash_index].setNext(index);
        }
        table[hash_index].setPrev(index);
        if (table[hash_index].getNext() == hash_index) {
            table[hash_index].setNext(index);
        }
        size++;
    }

    virtual void remove(const T1 &key) {
        auto hash_index = hash(key) % capacity;
        auto element = find(key);
        if (element == end()) {
            throw std::out_of_range("Key not found");
        }
        if ((*element).key == table[hash_index].key) {
            auto next = table[hash_index].getNext();
            table[hash_index].key = table[next].key;
            table[hash_index].value = table[next].value;
            table[hash_index].setNext(table[next].getNext());
            table[next].setNotTaken();
            auto new_next = table[hash_index].getNext();
            table[new_next].setPrev(hash_index);
        }
        else {
            auto next = (*element).getNext();
            table[next].setPrev((*element).getPrev());
            (*element).setNotTaken();
        }
        --size;
    }

    virtual bool contains(const T1 &key) {
        return find(key) != end();
    }

    virtual size_t getSize() const {
        return size;
    }

    virtual size_t getCapacity() const {
        return capacity;
    }

    virtual void clear() {
        size = 0;
        capacity = 0;
        delete[] table;
        table = nullptr;
    }
    virtual ~HashTable() {
        clear();
    }
};

#endif //LAB3_HASH_TABLE_H
