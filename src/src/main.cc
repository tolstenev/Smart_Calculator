#include "controller/main_controller.h"
#include "view/main_window.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  s21::CalculatorModel model;
  s21::CalculatorController controller(model);
  s21::CalculatorWindow view(controller);
  view.show();

  return app.exec();
}
