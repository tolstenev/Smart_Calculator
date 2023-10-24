#ifndef SRC_VIEW_MAIN_WINDOW_H
#define SRC_VIEW_MAIN_WINDOW_H

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

class CalculatorWindow : public QMainWindow {
  Q_OBJECT

 public:
  CalculatorWindow(CalculatorController &controller, QWidget *parent = nullptr);
  ~CalculatorWindow();

  void keyPressEvent(QKeyEvent *event) override;

 private:
  Ui::MainWindow *ui_;
  CalculatorController &controller_;
  void ConnectSlots();
  void InitPlot();
  void FormatPlotLine();

 private slots:
  void PrintSymbols();
  void ClearAll();
  void DeleteLastSymbol();
  void Calculate();
  void CreatePlot();

};  // class CalculatorWindow

}  // namespace s21

#endif  // SRC_VIEW_MAIN_WINDOW_H
