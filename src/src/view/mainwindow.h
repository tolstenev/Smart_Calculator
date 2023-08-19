#ifndef SRC_VIEW_MAINWINDOW_H
#define SRC_VIEW_MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace s21 {

    class CalculatorWindow : public QMainWindow {
    Q_OBJECT

    public:
        CalculatorWindow(QWidget *parent = nullptr);

        ~CalculatorWindow();

        void keyPressEvent(QKeyEvent *event) override;

    private:
        Ui::MainWindow *ui_;
        void connectSlots();

    private slots:
        void typeChars();
    };  // class CalculatorWindow

}   // namespace s21

#endif // SRC_VIEW_MAINWINDOW_H
