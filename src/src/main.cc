#include "controller/main_controller.h"
#include "view/main_window.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  s21::CalcModel model;
  s21::CalcController controller(model);
  s21::CalcWindow view(controller);
  view.show();

  return app.exec();
}
