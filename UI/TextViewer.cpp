#include <QTextBrowser>
#include <QFileDialog>
#include <QHBoxLayout>
#include "TextViewer.h"
#include "../AlphabetIndex.h"


#include <QApplication>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>

TextViewer::TextViewer(QWidget *parent ):QMainWindow(parent), currentPageIndex(0),rowSize(6),pageSize(6) {
    setupUI();
}
void TextViewer::loadText() {
    fileName = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    std::ifstream inputFile(fileName.toStdString());
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

    book = new Book(pages);

    currentPageIndex = 0;
    displayPage(currentPageIndex);
}

void TextViewer::saveIndex() {
    outputIndexFile = QFileDialog::getSaveFileName(this, "Output index file", "", "Text Files (*.txt)");
}
void TextViewer::saveBook() {
    outputPageFile = QFileDialog::getSaveFileName(this, "Output pages file", "", "Text Files (*.txt)");
}

void TextViewer::nextPage() {
    if (book != nullptr && currentPageIndex<book->GetPageCount() - 1) {
            currentPageIndex++;
            displayPage(currentPageIndex);

        }

}

void TextViewer::previousPage() {
    if (book != nullptr && currentPageIndex > 0) {
        currentPageIndex--;
        displayPage(currentPageIndex);
    }
}

void TextViewer::setupUI() {
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(centralWidget);
    auto *layout = new QVBoxLayout(this);
    auto *indexLayout = new QVBoxLayout(this);
    mainLayout->addSpacing(5);
    mainLayout->addLayout(indexLayout,1);

    QFrame *verticalLine= new QFrame();
    verticalLine->setFrameShape(QFrame::VLine);
    verticalLine->setFrameShadow(QFrame::Sunken); // Опционально: делает вид более трёхмерным
    verticalLine->setStyleSheet("background-color: #808080;"); // Серая полоска
    verticalLine->setFixedWidth(2); // Толщина полоски
    mainLayout->addWidget(verticalLine);

    mainLayout->addLayout(layout,2);
    mainLayout->addSpacing(5);
    textBrowser = new QTextBrowser(this);
    layout->addWidget(textBrowser);

    auto *pageIndicatorLayout = new QHBoxLayout();
    pageIndicator = new QLabel();
    pageIndicator->setText(QString("%1").arg(currentPageIndex));
    pageIndicator->setAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    pageIndicatorLayout->addWidget(pageIndicator);
    layout->addLayout(pageIndicatorLayout);

    QFrame *horizontalLine1 = new QFrame();
    horizontalLine1->setFrameShape(QFrame::HLine);
    horizontalLine1->setFrameShadow(QFrame::Sunken); // Опционально: делает вид более трёхмерным
    horizontalLine1->setStyleSheet("background-color: #808080;"); // Серая полоска
    horizontalLine1->setFixedHeight(2); // Толщина полоски
    layout->addWidget(horizontalLine1);

    auto *firstButtonLayout = new QHBoxLayout();
    auto *prevButton = new QPushButton("Предыдущая страница", this);
    auto *nextButton = new QPushButton("Следующая страница", this);
    firstButtonLayout->addWidget(prevButton);
    firstButtonLayout->addWidget(nextButton);
    layout->addLayout(firstButtonLayout);
    connect(prevButton, &QPushButton::clicked, this, &TextViewer::previousPage);
    connect(nextButton, &QPushButton::clicked, this, &TextViewer::nextPage);


    QFrame *horizontalLine2 = new QFrame();
    horizontalLine2->setFrameShape(QFrame::HLine);
    horizontalLine2->setFrameShadow(QFrame::Sunken); // Опционально: делает вид более трёхмерным
    horizontalLine2->setStyleSheet("background-color: #808080;"); // Серая полоска
    horizontalLine2->setFixedHeight(2); // Толщина полоски
    layout->addWidget(horizontalLine2);

    auto *secondButtonLayout = new QHBoxLayout();
    auto *loadButton = new QPushButton("Файл загрузки", this);
    auto *saveIndexButton = new QPushButton("Файл сохранения индекса", this);
    auto *saveBookButton = new QPushButton("Файл сохранения книги", this);
    secondButtonLayout->addWidget(loadButton);
    secondButtonLayout->addWidget(saveIndexButton);
    secondButtonLayout->addWidget(saveBookButton);
    layout->addLayout(secondButtonLayout);
    connect(loadButton, &QPushButton::clicked, this, &TextViewer::loadText);
    connect(saveIndexButton, &QPushButton::clicked, this, &TextViewer::saveIndex);
    connect(saveBookButton, &QPushButton::clicked, this, &TextViewer::saveBook);

    QFrame *horizontalLine3 = new QFrame();
    horizontalLine3->setFrameShape(QFrame::HLine);
    horizontalLine3->setFrameShadow(QFrame::Sunken); // Опционально: делает вид более трёхмерным
    horizontalLine3->setStyleSheet("background-color: #808080;"); // Серая полоска
    horizontalLine3->setFixedHeight(2); // Толщина полоски
    layout->addWidget(horizontalLine3);


    auto *inNumberLayout = new QHBoxLayout();
    inRowNumberInput = new QSpinBox(this);
    inRowNumberInput->setMinimum(1);
    inRowNumberInput->setMaximum(100);
    inRowNumberInput->setValue(rowSize);
    inPageNumberInput = new QSpinBox(this);
    inPageNumberInput->setMinimum(1);
    inPageNumberInput->setMaximum(1000);
    inPageNumberInput->setValue(pageSize);
    auto *submitButton = new QPushButton("Разбиение", this);

    inNumberLayout->addWidget(inRowNumberInput);
    inNumberLayout->addWidget(inPageNumberInput);
    inNumberLayout->addWidget(submitButton);
    layout->addLayout(inNumberLayout);


    connect(submitButton, &QPushButton::clicked, this, &TextViewer::setParameters);

    setCentralWidget(centralWidget);

    // Кнопка для создания списка
    QPushButton *createListButton = new QPushButton("Создать индекс", this);
    indexLayout->addWidget(createListButton);

    // Скроллируемая область для списка элементов
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    // Виджет-контейнер для кнопок
    listContainer = new QWidget(scrollArea);
    listLayout = new QVBoxLayout(listContainer);
    listContainer->setLayout(listLayout);

    scrollArea->setWidget(listContainer);
    indexLayout->addWidget(scrollArea);

    connect(createListButton, &QPushButton::clicked, this, generateAlphabetIndex);

}

void TextViewer::displayPage(int pageIndex) {
    if (pageIndex < 0 || pageIndex >= book->GetPageCount()) return;
    currentPageIndex = pageIndex;
    const Page &page = book->GetPage(pageIndex);
    std::cout<<"book size"<<book->GetPageCount();
    QString pageText;
    for (size_t i = 0; i < page.getPageSize(); ++i) {
        const Row &row = page.getRow(i);
        pageText += QString::fromStdString(row.get()) + "\n";
    }
    pageIndicator->setText(QString("%1").arg(currentPageIndex+1));
    textBrowser->setText(pageText);
}

void TextViewer::setParameters() {
    rowSize = inRowNumberInput->text().toInt();
    pageSize = inPageNumberInput->text().toInt();
    if (!fileName.isEmpty()) {
        std::ifstream inputFile(fileName.toStdString());
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
        book = new Book(pages);
        currentPageIndex = 0;
        displayPage(currentPageIndex);
    }
}
void TextViewer::generateAlphabetIndex() {
    if (fileName.isEmpty() || outputIndexFile.isEmpty() || outputPageFile.isEmpty())
        return;
    for (auto& elem: buttons) {
        listLayout->removeWidget(elem.first);
    }
    buttons.clear();
    auto path = fileName.toStdString();
    delete sortIndex;
    sortIndex = new MutableSegmentedDeque<std::pair<std::string,std::vector<int>>>();
    alphabeticIndex = &buildAlphabeticalIndexFromFile(
        fileName.toStdString(),
        pageSize,
        outputIndexFile.toStdString(),
        outputPageFile.toStdString(),
        rowSize,
        sortIndex
        );

    for(int i=0;i<sortIndex->GetLength();i++) {
        int index = i;
        auto item = (*sortIndex)[i];
        auto buttonString = item.first;
        for (int j = 0; j < item.second.size(); ++j) {
            if (j==0 || item.second[j] != item.second[j-1]) {
                buttonString = buttonString + " " + std::to_string(item.second[j]);
            }

        }
        QPushButton *button = new QPushButton(QString::fromStdString(buttonString), this);
        buttons.push_back(std::make_pair(button,0));
        std::cout << item.first << std::endl;

        // Подключаем клик по кнопке к обработчику
        auto clickFunc = [=]() {
            displayPage(item.second[buttons[index].second]-1);
            std::cout <<" : "<< buttons[index].second << " "<<item.second[buttons[index].second]<< std::endl;
            int buf = item.second[buttons[index].second]-1 ;
            int counter = 0;
            do{
                buttons[index].second = (buttons[index].second + 1) %  item.second.size();
                std::cout << buttons[index].second << std::endl;
                counter++;
            }while(item.second[buttons[index].second]-1==buf && item.second.size()>1&&counter<item.second.size());

            highlightOccurrences(QString::fromStdString(item.first),Qt::cyan);
        };
        connect(button, &QPushButton::clicked,this, clickFunc);

        listLayout->addWidget(button);
    }
}

void TextViewer::highlightOccurrences(const QString& word, const QColor& color ) {
    if (!textBrowser || word.isEmpty()) {
        return; // Если указатель пустой или строка для поиска пуста, выходим
    }

    QTextDocument* document = textBrowser->document();
    QTextCursor cursor(textBrowser->document());
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(color);

    //cursor.beginEditBlock(); // Начинаем блок редактирования текста
    // Поиск и подсветка всех вхождений
    QString pattern = QString(R"(\b%1\b)").arg(QRegularExpression::escape(word));
    QRegularExpression regex(pattern);
    while (!cursor.isNull() && !cursor.atEnd()) {
        cursor = document->find(regex, cursor); // Ищем слово в тексте

        if (!cursor.isNull()) {
            cursor.setCharFormat(highlightFormat); // Подсвечиваем найденное слово
        }
    }

}

