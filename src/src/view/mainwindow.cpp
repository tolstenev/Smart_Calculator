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
    auto *button = qobject_cast<QPushButton *>(sender());
    QString new_result = ui_->line_expr->text() + button->text();
    ui_->line_expr->setText(new_result);
}

void s21::CalculatorWindow::connectSlots() {
    std::list<QPushButton *> entering_buttons = {ui_->button_0,
                                                 ui_->button_1,
                                                 ui_->button_2,
                                                 ui_->button_3,
                                                 ui_->button_4,
                                                 ui_->button_5,
                                                 ui_->button_6,
                                                 ui_->button_7,
                                                 ui_->button_8,
                                                 ui_->button_9,
                                                 ui_->button_open_bracket,
                                                 ui_->button_close_bracket,
                                                 ui_->button_sum,
                                                 ui_->button_sub,
                                                 ui_->button_mul,
                                                 ui_->button_div,
                                                 ui_->button_dot,
                                                 ui_->button_degree,
                                                 ui_->button_x,
                                                 ui_->button_e,
                                                 ui_->button_sin,
                                                 ui_->button_cos,
                                                 ui_->button_tan,
                                                 ui_->button_asin,
                                                 ui_->button_acos,
                                                 ui_->button_atan,
                                                 ui_->button_log,
                                                 ui_->button_ln,
                                                 ui_->button_mod,
                                                 ui_->button_sqrt,};

    for (auto button: entering_buttons) {
        connect(button, SIGNAL(clicked()), this, SLOT(typeChars()));
    }

    connect(ui_->button_ac, SIGNAL(clicked()), this, SLOT(clearLine()));
    connect(ui_->button_bs, SIGNAL(clicked()), this, SLOT(deleteLastSymbol()));
}

void s21::CalculatorWindow::clearLine() {
    ui_->line_expr->setText("");
}

void s21::CalculatorWindow::deleteLastSymbol() {
    QString new_result = ui_->line_expr->text();
    new_result = new_result.left(new_result.length() - 1);
    ui_->line_expr->setText(new_result);
}

