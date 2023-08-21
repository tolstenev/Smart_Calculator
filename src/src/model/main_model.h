#ifndef SRC_MODEL_MAIN_MODEL_H_
#define SRC_MODEL_MAIN_MODEL_H_

#include <map>
#include <string>
#include "../rcs/exprtk.hpp"

namespace s21 {
    class CalcModel {
    public:
        CalcModel();

//        std::string getResult() { return expr_; }
        std::string getResult() { return std::to_string(result_); }


        void setExpression(const std::string &expr);

        void validateExpression();


    private:
        enum class Lexem : int {
            sin = 1,
            cos,
            tan,
            a_sin,
            a_cos,
            a_tan,
            sqrt,
            log,
            log10,
            br_open,
            br_close,
            deg,
            mul,
            div,
            sum,
            sub,
            mod,
            unary,
            num,
            num_x
        };
        std::string expr_{};
        double result_{};

        std::map<std::string, Lexem> functions_;
        std::map<char, Lexem> operators_;
        std::map<Lexem, int> priorities_;

        void mapsObjectsInit();

    };  // class CalcModel
}  // namespace s21

#endif  // SRC_MODEL_MAIN_MODEL_H_