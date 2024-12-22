#ifndef LAB3_ALPHABET_INDEX
#define LAB3_ALPHABET_INDEX
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include "Dictionary.h" 
#include "Book.h"
#include "Pages.h"
#include "Rows.h"
#include "Sequences/Sequence.h"
#include "Sequences/SegmentedDeque.h"
#include "Sorting/heapSort.h"
#include "Sorting/DefaultComparators.h"

Dictionary<std::string, std::vector<int>>& buildAlphabeticalIndexFromFile(
    const std::string &inputFilePath,
    int pageSize,
    const std::string &outputIndexPath,
    const std::string &outputBookPath,
    int rowSize,
    Sequence<std::pair<std::string,std::vector<int>>>* sortedIndex = nullptr
    ){
    
    auto index = new Dictionary<std::string, std::vector<int>>;

    // Чтение текста из входного файла
    std::ifstream inputFile(inputFilePath);
    if (!inputFile) {
        throw std::runtime_error("Не удалось открыть входной файл.");
    }

    std::ostringstream textBuffer;
    textBuffer << inputFile.rdbuf();
    inputFile.close();
    std::string inputText = textBuffer.str();

    // Замена табуляций и переносов строк на пробелы
    for (char &ch : inputText) {
        if (ch == '\n' || ch == '\t' || !(std::isalnum(ch))) {
            ch = ' ';
        }
        else
        {
            ch = static_cast<char>(std::tolower(ch));
        }
    }

    DynamicArray<Row> rows = lexer(inputText, rowSize);

    DynamicArray<Page> pages = PageMaker(rows, pageSize);
    
    Book item(pages);
    std::ofstream bookOutputFile(outputBookPath);
    if (!bookOutputFile) {
        throw std::runtime_error("Не удалось открыть файл для записи книги.");
    }

    bookOutputFile << "Book:\n";
    for (int i = 0; i < item.GetPageCount(); i++) {
        bookOutputFile << "Page " << i + 1 << ":\n";

        Page page = item.GetPage(i);

        for (int j = 0; j < item.GetPage(i).getPageSize(); j++) {
            bookOutputFile << page.getRow(j).get() << '\n';
        }
        bookOutputFile << '\n';
    }

    bookOutputFile.close();

    std::istringstream wordsStream(inputText);
    std::vector<std::string> words;
    std::string word;
    while (wordsStream >> word) {
        words.push_back(word);
    }


    for (int NumPage = 0; NumPage < item.GetPageCount(); NumPage++)
    {
        Page page = item.GetPage(NumPage);
        for (int NumRow = 0; NumRow < page.getPageSize(); NumRow++)
        {
            Row row = page.getRow(NumRow);
            std::istringstream rowStream(row.get());
            std::string word;
            while(rowStream >> word)
            {
                if(index->contains(word))
                {
                    std::vector<int> &pages = index->get(word);
                    if (pages.empty() || pages.back() != NumPage)
                    {
                        pages.push_back(NumPage + 1);
                    }
                }
                else
                {
                    index->add(word, {NumPage + 1});
                }
            }
        }
        
    }
    
    std::ofstream outputFile(outputIndexPath);
    if (!outputFile) {
        throw std::runtime_error("Не удалось открыть выходной файл.");
    }
    if (sortedIndex==nullptr) {
        sortedIndex = new MutableSegmentedDeque<std::pair<std::string, std::vector<int>>>;
    }
    for (auto it = index->begin(); it != index->end(); ++it) {
        sortedIndex->Append(*it);
    }
    auto sorter = HeapSorter<std::pair<std::string, std::vector<int>>>();
    sorter.Sort(*sortedIndex,[&](
        std::pair<std::string, std::vector<int>> a,
        std::pair<std::string, std::vector<int>> b
        )->int {
            return ascendingComparator(a.first, b.first);
        }
    );
    for (int j=0; j <sortedIndex->GetLength();j++) {
        outputFile << sortedIndex->Get(j).first << ": ";
        for (size_t i = 0; i < sortedIndex->Get(j).second.size(); ++i) {
            outputFile << sortedIndex->Get(j).second[i];
            if (i <sortedIndex->Get(j).second.size() - 1) {
                outputFile << ", ";
            }
        }
        outputFile << std::endl;
    }

    outputFile.close();
    return *index;
}

#endif //LAB3_ALPHABET_INDEX
