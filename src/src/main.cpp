#include "src/view/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    s21::CalculatorWindow cw;
    cw.show();
    return a.exec();
}
