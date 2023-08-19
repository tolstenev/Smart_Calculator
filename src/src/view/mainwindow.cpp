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
    connect(ui_->button_1, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_2, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_3, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_4, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_5, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_6, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_7, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_8, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_9, SIGNAL(clicked()), this, SLOT(typeChars()));

    connect(ui_->button_open_bracket, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_close_bracket, SIGNAL(clicked()), this, SLOT(typeChars()));

    connect(ui_->button_sum, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_sub, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_mul, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_div, SIGNAL(clicked()), this, SLOT(typeChars()));

    connect(ui_->button_dot, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_degree, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_x, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_sin, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_cos, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_tan, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_asin, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_acos, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_atan, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_log, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_ln, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_mod, SIGNAL(clicked()), this, SLOT(typeChars()));
    connect(ui_->button_sqrt, SIGNAL(clicked()), this, SLOT(typeChars()));


}




