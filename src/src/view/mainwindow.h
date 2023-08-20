#ifndef SRC_VIEW_MAINWINDOW_H
#define SRC_VIEW_MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <list>

#include "../controller/main_controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace s21 {

    class CalcWindow : public QMainWindow {
    Q_OBJECT

    public:
        CalcWindow(CalcController& controller, QWidget *parent = nullptr);

        ~CalcWindow();

        void keyPressEvent(QKeyEvent *event) override;

    private:
        Ui::MainWindow *ui_;
        CalcController& controller_;
        void connectSlots();

    private slots:
        void printSymbols();
        void clearLine();
        void deleteLastSymbol();
        void calculate();

    };  // class CalculatorWindow

}   // namespace s21



#endif // SRC_VIEW_MAINWINDOW_H
