#include "mainwindow.h"
#include "./ui_mainwindow.h"

CalculatorWindow::CalculatorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("SmartCalc by Yonn Argelia @yonnarge");
}

CalculatorWindow::~CalculatorWindow()
{
    delete ui;
}

void CalculatorWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_W)
    {
        close(); // Закрытие окна по сочетанию клавиш Cmd+W
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}