#ifndef LAB3_PAGES
#define LAB3_PAGES
#pragma once

#include "Rows.h"
#include "array.h"
class Page
{
private:
    DynamicArray<Row> page_;
public:
    Page() = default;

    Page(DynamicArray<Row>& page)
    {
        page_ = page;
    }
    Page(DynamicArray<Row>&& other) noexcept
        : page_(std::move(other)) {};

    void append(Row& line)
    {
        page_.push_back(line);
    }

    Row getRow(int index) const
    {
        return page_[index];
    }

    size_t getPageSize() const
    {
        return page_.get_size();
    }
    ~Page() = default;
};


DynamicArray<Page> PageMaker(DynamicArray<Row>& rows, int page_size);

#endif //LAB3_PAGES
