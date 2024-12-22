
#include "Pages.h"
DynamicArray<Page> PageMaker(DynamicArray<Row>& rows, int page_size)
{
    DynamicArray<Page> result;
    int count = 0;
    DynamicArray<Row> buf;
    for (int i = 0; i < rows.get_size(); i++)
    {
        buf.push_back(rows[i]);
        count++;
        if (count == page_size)
        {
            Page item(buf);
            result.push_back(item);
            buf.erase(buf.begin(), buf.end());
            count = 0;
        }
    }
    if (buf.get_size() != 0)
    {
        Page item2(buf);
        result.push_back(std::move(item2));
        buf.erase(buf.begin(), buf.end());
    }
    return result;
}