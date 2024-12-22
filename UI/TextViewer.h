#pragma once



// Объявления классов из вашего кода
#include <QLabel>
#include <QLineEdit>
#include <qtmetamacros.h>
#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QTextBrowser>
#include <QScrollArea>
#include <QPushButton>
#include "../Book.h"
#include "../Dictionary.h"
#include "../Sequences/Sequence.h"
#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H



// Пример класса интерфейса
class TextViewer : public QMainWindow {
    Q_OBJECT

public:
    TextViewer(QWidget *parent = nullptr);

private slots:
    void loadText();

    void nextPage();

    void previousPage();

    void setParameters();

    void generateAlphabetIndex();

    void saveBook();
    void saveIndex();
    void highlightOccurrences(const QString& word, const QColor& color = Qt::yellow);
private:
    void setupUI();

    void displayPage(int pageIndex);

    QString fileName;
    QString outputPath;
    QString outputIndexFile;
    QString outputPageFile;
    QTextBrowser *textBrowser;
    QSpinBox* inRowNumberInput;
    QSpinBox* inPageNumberInput;
    QLabel* pageIndicator  ;

    Dictionary<std::string,std::vector<int>>*  alphabeticIndex;
    Sequence<std::pair<std::string,std::vector<int>>>* sortIndex = nullptr;
    QScrollArea* scrollArea ;
    QWidget* listContainer ;
    QVBoxLayout* listLayout ;
    Book* book = nullptr;
    std::vector<std::pair<QPushButton*,int>> buttons;


    int currentPageIndex;
    int rowSize;
    int pageSize;
};



#endif //TEXTVIEWER_H
