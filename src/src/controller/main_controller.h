#ifndef SRC_CONTROLLER_MAIN_CONTROLLER_H_
#define SRC_CONTROLLER_MAIN_CONTROLLER_H_

#include "../model/main_model.h"

namespace s21 {
    class CalcController {
    public:
        CalcController(CalcModel &model) : model_(model) {}

        void setExpression(const std::string &expr) { model_.setExpression(expr); }

        void validateExpression() { model_.validateExpression(); }

        void calculateExpression() { model_.calculateExpression(); }

        std::string getResult() { return model_.getResult(); }

    private:
        CalcModel &model_;
    };  // class CalcController
}  // namespace s21

#endif  // SRC_CONTROLLER_MAIN_CONTROLLER_H_
