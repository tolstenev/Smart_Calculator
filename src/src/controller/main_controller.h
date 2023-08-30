#ifndef SRC_CONTROLLER_MAIN_CONTROLLER_H_
#define SRC_CONTROLLER_MAIN_CONTROLLER_H_

#include "../model/main_model.h"

namespace s21 {
class CalcController {
 public:
  CalcController(CalcModel &model) : model_(model) {}

  void setXValue(double x) { model_.setXValue(x); };

  std::string calculate(const std::string &expression) {
    model_.calculate(expression);
    return model_.getResultString();
  };

  std::pair<std::vector<double>, std::vector<double>> calculateDots(
      const std::string &expression, std::vector<double> plot_limits) {
    model_.calculateDots(expression, plot_limits);
    return model_.getDots();
  };

 private:
  CalcModel &model_;
};  // class CalcController
}  // namespace s21

#endif  // SRC_CONTROLLER_MAIN_CONTROLLER_H_
