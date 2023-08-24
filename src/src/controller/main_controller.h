//#ifndef SRC_CONTROLLER_MAIN_CONTROLLER_H_
//#define SRC_CONTROLLER_MAIN_CONTROLLER_H_
//
//#include "../model/main_model.h"
//
//namespace s21 {
//    class CalcController {
//    public:
//        CalcController(CalcModel &model) : model_(model) {}
//
//        void setExpression(const std::string &Expression) { model_.setExpression(Expression); }
//
//        void setXvalue(double x) { model_.setXvalue(x); };
//
//        void validateExpression() { model_.validateExpression(); }
//
//        void convertExpressionToPostfix() { model_.convertExpressionToPostfix(); }
//
//        void calculateExpression() { model_.calculateExpression(); };
//
//        std::string getResult() { return model_.getResult(); }
//
//    private:
//        CalcModel &model_;
//    };  // class CalcController
//}  // namespace s21
//
//#endif  // SRC_CONTROLLER_MAIN_CONTROLLER_H_


#ifndef SRC_CONTROLLER_MAIN_CONTROLLER_H_
#define SRC_CONTROLLER_MAIN_CONTROLLER_H_

#include "../model/main_model.h"

namespace s21 {
    class CalcController {
    public:
        CalcController(CalcModel &model) : model_(model) {}

        std::string calculateExpression(const std::string &expression) {
            try {
                model_.setExpression(expression);
//            model_.setXvalue(x);
                model_.validateExpression();
                model_.convertExpressionToPostfix();
                model_.calculateExpression();
                return model_.getResult();
            } catch (...) {
                return "Error";
            }
        };

    private:
        CalcModel &model_;
    };  // class CalcController
}  // namespace s21

#endif  // SRC_CONTROLLER_MAIN_CONTROLLER_H_
