#ifndef LAB3_BOOK_H
#define LAB3_BOOK_H
#pragma once
#include "Pages.h"
#include "array.h"

class Book
{
private:
    DynamicArray<Page> book;
public:
    Book() = default;

    void append(Page& insert_page)
    {
        book.push_back(insert_page);
    }

    Book(DynamicArray<Page>& book)
    {
        this->book = book;
    }

    Book(DynamicArray<Page>&& other) noexcept
        : book(std::move(other)) {};
    
    size_t GetPageCount()
    {
        return book.get_size();
    }
    
    Page GetPage(int i)
    {
        return book[i];
    }
};

#endif //LAB3_BOOK_H
