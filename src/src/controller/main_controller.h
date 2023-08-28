#ifndef SRC_CONTROLLER_MAIN_CONTROLLER_H_
#define SRC_CONTROLLER_MAIN_CONTROLLER_H_

#include "../model/main_model.h"

namespace s21 {
class CalcController {
 public:
  CalcController(CalcModel &model) : model_(model) {}

  void setXvalue(double x) { model_.setXValue(x); };

  std::string calculateExpression(const std::string &expression) {
    return model_.getResult(expression);
  };

 private:
  CalcModel &model_;
};  // class CalcController
}  // namespace s21

#endif  // SRC_CONTROLLER_MAIN_CONTROLLER_H_
