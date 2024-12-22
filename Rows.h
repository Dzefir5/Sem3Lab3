#ifndef LAB3_ROWS
#define LAB3_ROWS

#include <string>

#include "array.h"

class Row
{
private:
    std::string row_;
public:
    Row() = default;
    
    Row(std::string& get_row)
    {
        row_ = get_row;
    }
    
    void append(std::string& word)
    {
        row_ = row_ + word;
    }
    
    const std::string& get() const
    {
        return row_;
    }
};

DynamicArray<Row> lexer(std::string& text, int row_size);

#endif //LAB3_ROWS
