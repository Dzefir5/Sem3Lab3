#ifndef LAB3_TESTS
#define LAB3_TESTS
#include <iostream>
#include <cassert>
#include "HashTable.h"
#include <iterator>







struct TestStruct {
    int *value;
    TestStruct() : value(new int(0)) {}
    TestStruct(int value) : value(new int(value)) {}
    TestStruct(const TestStruct &other) : value(new int(*other.value)) {}
    TestStruct(TestStruct &&other) : value(other.value) {
        other.value = nullptr;
    }

    TestStruct &operator=(const TestStruct &other) {
        if (this == &other) {
            return *this;
        }
        delete value;
        value = new int(*other.value);
        return *this;
    }

    TestStruct &operator=(TestStruct &&other) {
        if (this == &other) {
            return *this;
        }
        delete value;
        value = other.value;
        other.value = nullptr;
        return *this;
    }
    TestStruct &operator=(int value) {
        delete this->value;
        this->value = new int(value);
        return *this;
    }
    ~TestStruct() {
        delete value;
    }

    int operator*() {
        return *value;
    }
};

void testDefaultConstructor() {
    HashTable<int, TestStruct> table;
    assert(table.getSize() == 0);
    assert(table.getCapacity() == 0);
    
}

void testParameterizedConstructor() {
    HashTable<int, TestStruct> table(10);
    assert(table.getSize() == 0);
    assert(table.getCapacity() == 10);
    
}

void testInsert() {
    HashTable<int, TestStruct> table(10);
    table.insert(1, 1);
    table.insert(2, 2);
    assert(table.getSize() == 2);
    assert(table.getCapacity() == 10);
    assert(*(*(table.find(1))).value == 1);
    assert(*(*(table.find(2))).value == 2);
    
}

void testRemoveExistingKey() {
    HashTable<int, TestStruct> table(10);
    table.insert(1, 1);
    table.insert(2, 2);
    table.insert(3, 3);

    table.remove(1);
    assert(table.getSize() == 2);
    assert(table.find(1) == table.end());

    table.remove(2);
    assert(table.getSize() == 1);
    assert(table.find(2) == table.end());
   
}

void testContains() {
    HashTable<int, TestStruct> table(10);
    table.insert(1, 1);
    table.insert(2, 2);
    assert(table.contains(1));
    assert(table.contains(2));
    assert(!table.contains(3));
    
}

void testSizeAndCapacity() {
    HashTable<int, TestStruct> table(10);
    table.insert(1, 1);
    table.insert(2, 2);
    assert(table.getSize() == 2);
    assert(table.getCapacity() == 10);
    
}

void testClear() {
    HashTable<int, TestStruct> table(10);
    table.insert(1, 1);
    table.insert(2, 2);
    table.clear();
    assert(table.getSize() == 0);
    assert(table.getCapacity() == 0);
    
}

void testResize() {
    HashTable<int, TestStruct> table(2);
    table.insert(1, 1);
    table.insert(2, 2);
    table.insert(3, 3);
    assert(table.getSize() == 3);
    assert(table.getCapacity() == 4);
    
}

void testIterator() {
    HashTable<int, TestStruct> table(10);
    table.insert(1, 1);
    table.insert(2, 2);
    auto it = table.begin();
    assert((*it).key == 1);
    assert(*((*it).value) == 1);
    ++it;
    assert((*it).key == 2);
    assert(*((*it).value) == 2);
    
}

void testFind() {
    HashTable<int, TestStruct> table(10);
    table.insert(1, 1);
    table.insert(2, 2);
    auto it = table.find(1);
    assert((*it).key == 1);
    assert(*((*it).value) == 1);
    
}

void testOperatorBracket() {
    HashTable<int, TestStruct> table(10);
    table.insert(1, 1);
    table.insert(2, 2);
    assert(*(table[1]) == 1);
    assert(*(table[2]) == 2);
    
}

void testCopyAssignment() {
    HashTable<int, TestStruct> hashTable;
    hashTable.insert(-1, 1);
    hashTable.insert(-2, 2);

    HashTable<int, TestStruct> anotherHashTable;
    anotherHashTable = hashTable;

    assert(anotherHashTable.getSize() == 2);
    assert(*(anotherHashTable[-1]) == 1);
    assert(*(anotherHashTable[-2]) == 2);
    
}

void testMoveAssignment() {
    HashTable<int, TestStruct> hashTable;
    hashTable.insert(-1, 1);
    hashTable.insert(-2, 2);

    HashTable<int, TestStruct> anotherHashTable;
    anotherHashTable = std::move(hashTable);

    assert(anotherHashTable.getSize() == 2);
    assert(*(anotherHashTable[-1]) == 1);
    assert(*(anotherHashTable[-2]) == 2);
}



void startTests()
{
    testDefaultConstructor();
    testParameterizedConstructor();
    testInsert();
    testRemoveExistingKey();
    testContains();
    testSizeAndCapacity();
    testClear();
    testResize();
    testIterator();
    testFind();
    testOperatorBracket();
    testCopyAssignment();
    testMoveAssignment();
}


#endif //LAB3_TESTS
