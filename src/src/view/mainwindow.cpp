#include "mainwindow.h"
#include "./ui_mainwindow.h"

namespace s21 {


    CalculatorWindow::CalculatorWindow(QWidget *parent)
            : QMainWindow(parent), ui_(new Ui::MainWindow) {
        ui_->setupUi(this);
        setWindowTitle("SmartCalc by Yonn Argelia @yonnarge");
    }

    CalculatorWindow::~CalculatorWindow() {
        delete ui_;
    }

    void CalculatorWindow::keyPressEvent(QKeyEvent *event) {
        if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_W) {
            close();  // Закрытие окна по сочетанию клавиш Cmd+W
        } else {
            QMainWindow::keyPressEvent(event);
        }
    }

}   // namespace s21
