#ifndef SRC_MODEL_MAIN_MODEL_H_
#define SRC_MODEL_MAIN_MODEL_H_

#include <sstream>
#include <map>
#include <queue>
#include <unordered_map>
#include <string>
#include <iostream>
#include "../rcs/exprtk.hpp"

namespace s21 {
class CalcModel {
 public:
  CalcModel();

  void setXValue(double x) { x_value_ = x; };
  std::string getResult(const std::string &expression);
  std::pair<std::vector<double>, std::vector<double>> calculateDots(double x_min,
                                                                    double x_max,
                                                                    double y_min,
                                                                    double y_max);

 private:
  enum class Lexem : int {
    sin = 1, cos, tan,
    aSin, aCos, aTan,
    sqrt, log, log10,
    deg, mul, div, sum, sub, mod,
    unaryMinus, brOpen,
    num, num_x
  };

  enum class LType : int {
    num, func, op
  };

  class Token {
   public:
    Token() = default;

    Token(LType type, Lexem name)
        : type_(type), name_(name), value_{} {}

    Token(LType type, Lexem name, double value)
        : type_(type), name_(name), value_(value) {}

    LType getType() const { return type_; }

    Lexem getName() const { return name_; }

    double getValue() const { return value_; }

   private:
    LType type_{};
    Lexem name_{};
    double value_{};
  };  // class Token

  std::string expr_{};
  std::string result_string_ = "Error";
  double x_value_{};
  double result_{};
  bool expect_unary_operator_{};
  std::vector<Token> postfix_{};
  std::unordered_map<std::string, Lexem> functions_;
  std::unordered_map<char, Lexem> operators_;
  std::unordered_map<Lexem, int> priorities_;
  std::stack<Token> stack_of_operators_;

  void setExpression(const std::string &expr) { expr_ = expr; };
  bool isExpressionValid();
  void convertExpressionToPostfix();
  void calculateExpression();
  void initFunctions();
  void initOperators();
  void initPriorities();
  void substituteExpression();
  void replaceInExpression(const std::string &from, const std::string &to);
  int getPriority(const Lexem &lexem);
  double extractDigit(size_t &pos);
  Lexem charToLexem(const char &oper);
  void handleVariable(size_t &index);
  void handleFunction(int func_type);
  void handleOpeningBrace(size_t &index);
  void handleClosingBrace(size_t &index);
  void handleNumeric(size_t &index);
  void handleOperator(size_t &index);
  bool isVariable(size_t index) const;
  int isFunction(size_t &index);
  bool isOpeningBrace(size_t index) const;
  bool isClosingBrace(size_t index) const;
  bool isNumeric(size_t index) const;
  void stackOfOperatorsToVector();
  void calculatePostfix();
  bool isX(const Token &token);
  bool isNumber(const Token &token);
  bool isFunction(const Token &token);
  bool isOperation(const Token &token);
  void handleNumber(const Token &token, std::stack<double> &numbers);
  void handleFunction(const Token &token, std::stack<double> &numbers);
  void handleOperation(const Token &token, std::stack<double> &numbers);
  void convertResultToString();
  bool isResultError() const;
  void formatResultString();
  void setAccuracy();
  void trimTrailingZeros();
  void ClearStackOfOperators();

};  // class CalcModel
}  // namespace s21

#endif  // SRC_MODEL_MAIN_MODEL_H_