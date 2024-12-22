
#include "Rows.h"

#include <string>
#include <fstream>
#include <sstream>

#include "array.h"


DynamicArray<Row> lexer(std::string& text, int row_size)
{
    DynamicArray<Row> result;
    int count = 0;
    std::istringstream wordsStream(text);
    std::string line;
    std::string word;
    while (wordsStream >> word)
    {
        line = line + word + " ";
        count++;
        if (count == row_size)
        {
            Row item(line);
            result.push_back(item);
            line.clear();
            count = 0;
        }
    }
    if (!line.empty())
    {
        Row item2(line);
        result.push_back(item2);
        line.clear();
        count = 0;
    }
    return result;
}
