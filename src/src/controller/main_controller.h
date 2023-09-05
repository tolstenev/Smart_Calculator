#ifndef SRC_CONTROLLER_MAIN_CONTROLLER_H_
#define SRC_CONTROLLER_MAIN_CONTROLLER_H_

#include <utility>

#include "../model/main_model.h"

namespace s21 {

class CalculatorController {
 public:
  CalculatorController(CalculatorModel &model) : model_(model) {}

  void SetXValue(double x) { model_.SetXValue(x); };

  std::string Calculate(const std::string &expression) {
    model_.Calculate(expression);
    return model_.GetResultString();
  };

  std::pair<std::list<double>, std::list<double>> CalculateDots(
      const std::string &expression, std::vector<double> plot_limits) {
    model_.CalculateDots(expression, std::move(plot_limits));
    return model_.GetDots();
  };

 private:
  CalculatorModel &model_;

};  // class CalculatorController
}  // namespace s21

#endif  // SRC_CONTROLLER_MAIN_CONTROLLER_H_
