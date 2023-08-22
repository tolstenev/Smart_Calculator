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

        void setExpression(const std::string &expr) { expr_ = expr; };

        void setXvalue(double x) { x_value_ = x; };

        void validateExpression();

        void convertExpressionToPostfix();

//        void calculateExpression();

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

        enum class LType : int {
            num,
            func,
            op
        };

        class Token {
        public:
            Token(LType type, Lexem name)
                    : type_(type), name_(name), value_{} {}

            Token(LType type, Lexem name, double value)
                    : type_(type), name_(name), value_(value) {}

            LType getLexemType() const { return type_; }

            Lexem getLexemName() const { return name_; }

            double getLexemValue() const { return value_; }

        private:
            LType type_{};
            Lexem name_{};
            double value_{};
        };  // class Token


        std::string expr_{};
        double x_value_{};
        double result_{};

        std::map<std::string, Lexem> functions_;
        std::map<char, Lexem> operators_;
        std::map<Lexem, int> priorities_;
        std::vector<Token> postfix_vector_{};

        void initFunctions();

        void initOperators();

        void initPriorities();

        void substituteExpression();

        void replaceInExpression(const std::string &from, const std::string &to);

        int getPriority(const Lexem &lexem);

        double extractDigit(size_t &pos);

        Lexem charToLexem(const char &oper);

        void handleUnaryOperator(size_t &index, std::stack<Token> &operators, bool &unary_ind);

        void handleVariable(size_t &index, bool &unary_ind);

        void handleFunction(int func_type, std::stack<Token> &operators, bool &is_unary);

        void handleOpeningBrace(size_t &index, std::stack<Token> &operators, bool &unary_ind);

        void handleClosingBrace(size_t &index, std::stack<Token> &operators, bool &unary_ind);

        void handleNumeric(size_t &index, bool &unary_ind);

        void handleOperator(size_t &index, std::stack<Token> &operators, bool &unary_ind);

        bool isUnaryOperator(size_t index, bool &is_unary) const;

        bool isVariable(size_t index) const;

        int isFunction(size_t &index) const;

        bool isOpeningBrace(size_t index) const;

        bool isClosingBrace(size_t index) const;

        bool isNumeric(size_t index) const;

        void stackToVector(std::stack<Token> &operators);

    };  // class CalcModel
}  // namespace s21

#endif  // SRC_MODEL_MAIN_MODEL_H_