#ifndef SRC_MODEL_MAIN_MODEL_H_
#define SRC_MODEL_MAIN_MODEL_H_

#include <map>

namespace s21 {
    class CalcModel {
    public:
        std::string getResult() { return expr_; }

        double getResultD() { return result_; }

        void setExpression(const std::string &expr);

        void validateExpression();

        void calculateExpression();

    private:

        std::string expr_{};
        double result_{};

    };  // class CalcModel
}  // namespace s21

#endif  // SRC_MODEL_MAIN_MODEL_H_