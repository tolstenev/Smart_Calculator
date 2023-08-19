#include "mainwindow.h"
#include "./ui_mainwindow.h"


s21::CalculatorWindow::CalculatorWindow(QWidget *parent)
        : QMainWindow(parent), ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
    setWindowTitle("SmartCalc by Yonn Argelia @yonnarge");
    connectSlots();

}

s21::CalculatorWindow::~CalculatorWindow() {
    delete ui_;
}

void s21::CalculatorWindow::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_W) {
        close();  // Закрытие окна по сочетанию клавиш Cmd+W
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void s21::CalculatorWindow::typeChars() {
    auto *button = qobject_cast<QPushButton*>(sender());
    QString new_result = ui_->line->text() + button->text();
    ui_->line->setText(new_result);
}

void s21::CalculatorWindow::connectSlots() {
    connect(ui_->button_0, SIGNAL(clicked()), this, SLOT(typeChars()));

}




