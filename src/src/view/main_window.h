#ifndef SRC_VIEW_MAINWINDOW_H
#define SRC_VIEW_MAINWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>
#include <QShortcut>
#include <list>
#include <string>

#include "../controller/main_controller.h"
#include "../rcs/qcustomplot/qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class CalcWindow : public QMainWindow {
  Q_OBJECT

 public:
  CalcWindow(CalcController &controller, QWidget *parent = nullptr);

  ~CalcWindow();

  void keyPressEvent(QKeyEvent *event) override;

 private:
  Ui::MainWindow *ui_;
  CalcController &controller_;
  void connectSlots();
  void initPlot();
  void formatPlot();

 private slots:
  void printSymbols();
  void clearLines();
  void deleteLastSymbol();
  void calculate();
  void createPlot();

};  // class CalculatorWindow

}  // namespace s21

#endif  // SRC_VIEW_MAINWINDOW_H
