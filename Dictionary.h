#ifndef LAB3_DICTIONARY
#define LAB3_DICTIONARY
#pragma once
#include "HashTable.h"
#include "IDictionary.h"

template<typename Key, typename Value>
class Dictionary: public IDictionary<Key,Value>{
private:
    HashTable<Key, Value> hashTable;

public:
    Dictionary(size_t capacity = 8) : hashTable(capacity) {}

    void add(const Key &key, const Value &value) override {
        if (hashTable.contains(key)) {
            throw std::runtime_error("Key already exists");
        }
        hashTable.insert(key, value);
    }

    void remove(const Key &key) override {
        hashTable.remove(key);
    }

    Value &get(const Key &key) override {
        return hashTable[key];
    }

    bool contains(const Key &key) override {
        return hashTable.contains(key);
    }

    size_t size() override {
        return hashTable.getSize();
    }

    void clear() override {
        hashTable.clear();
    }

    class Iterator {
    private:
        typename HashTable<Key, Value>::Iterator it;
    public:
        Iterator(typename HashTable<Key, Value>::Iterator iterator) : it(iterator) {}

        std::pair<Key, Value> operator*() {
            return {(*it).key, (*it).value};
        }

        Iterator &operator++() {
            ++it;
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return it != other.it;
        }
    };

    Iterator begin() {
        return Iterator(hashTable.begin());
    }

    Iterator end() {
        return Iterator(hashTable.end());
    }

};



#endif //LAB3_DICTIONARY
