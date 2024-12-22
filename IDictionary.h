//
// Created by solo on 20.12.2024.
//
#pragma once


#ifndef IDICTIONARY_H
#define IDICTIONARY_H

template<typename Key, typename Value>
class IDictionary { //IDictionary
public:
    virtual void add(const Key &key, const Value &value);

    virtual void remove(const Key &key);

    virtual Value &get(const Key &key);
    virtual bool contains(const Key &key);

    virtual size_t size();

    virtual void clear();

    virtual ~IDictionary() = default;
};

#endif //IDICTIONARY_H
