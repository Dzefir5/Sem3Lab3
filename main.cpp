#include "tests.h"


#include <QApplication>

#include "UI/TextViewer.h"
void applyStyle(QApplication &app) {
    QString styleSheet = R"(
        QMainWindow {
            background-color: #E2F1F8;
            color: #E2F1F8;
            border: 2px solid #E2F1F8;
            border-radius: 0px;
            font : 'SF Pro Display' 16px #37474F;
        }

        QLabel {
            color: #1C313A;
            font-size: 18px;
        }

        QPushButton {
            background-color: #B0BEC5;
            color: #1C313A;
            padding: 10px;
            font-size: 14px;
            font-weight: bold;
            border: 2px solid #1C313A;
            border-radius: 10px;
            border-style: outset;
        }

        QPushButton:hover {
            background-color: #819CA9;
        }

        QPushButton:pressed {
            background-color: #1C313A;
        }

        QComboBox {
            background-color: #B0BEC5;
            color: #1C313A;
            border: 3px #1C313A;
            border-radius: 10px;
            padding: 5px;
            font-size: 14px;
        }

        QComboBox::drop-down {
            border: 3px;
            color: #1C313A;
            border-radius: 15px 10px 5px 0px;
            padding: 5px 5px 5px 5px;
            background-color: #B0BEC5;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: url(:/icons/down-arrow-white.png);
            color : #FAFAFA;
        }

        QComboBox QAbstractItemView {
            background-color: #B0BEC5;
            color: #1C313A;
            selection-background-color: #A7C0CD; /* Цвет выделения */
            selection-color: #263238; /* Цвет текста при выделении */
            border: 1px solid #B0BEC5;
            border-radius: 5px 5px 0px 0px;
            padding: 5px 5px 0px 0px;
        }

        QComboBox QAbstractItemView::item {
            outline: none; /* Убираем фиолетовый контур */
            border: none; /* Убираем границу */
            padding: 0; /* Убираем внутренние отступы */
            margin: 0; /* Убираем внешние отступы */
            background-color: transparent; /* Сброс фона, если нужно */
        }

        QLineEdit {
            background-color: #B0BEC5;
            color: #1C313A;
            border: 2px solid #34515E;
            padding: 5px;
            font-size: 14px;
            border: 1px solid #B0BEC5;
            border-radius: 5px 5px 0px 0px;
        }
        QScrollArea{
            background-color: #B0BEC5;
            color: #1C313A;
            border: 2px solid #34515E;
            padding: 5px;
            font-size: 14px;
            border: 1px solid #B0BEC5;
            border-radius: 5px 5px 0px 0px;
        }
        QSpinBox {
            background-color: #B0BEC5;
            color: #1C313A;
            border: 2px solid #34515E;
            padding: 5px;
            font-size: 14px;
            border: 1px solid #B0BEC5;
            border-radius: 5px 5px 0px 0px;
        }
        QTableWidget {
            background-color: #B0BEC5;
            color: #1C313A;
            border: 2px solid #34515E;
            padding: 5px;
            font-size: 14px;
            border: 1px solid #B0BEC5;
            border-radius: 5px 5px 0px 0px;
        }
        QTableWidget::section {
            background-color: #B0BEC5;
            color: #1C313A;
            border: 2px solid #34515E;
            padding: 5px;
            font-size: 14px;
            border: 1px solid #B0BEC5;
            border-radius: 5px 5px 0px 0px;
        }
    )";
    app.setStyleSheet(styleSheet);
}


int main(int argc, char *argv[]) {
    startTests();
    QApplication app(argc, argv);
    //отсортировать слова в индексе по алфавиту
    TextViewer viewer;
    applyStyle(app);
    viewer.resize(800, 600);
    viewer.show();

    return app.exec();
}
