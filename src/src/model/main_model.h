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
  void calculate(const std::string &expression);
  std::string getResultString() { return result_string_; };
  void calculateDots(const std::string &expression, std::vector<double> plot_limits);
  std::pair<std::vector<double>, std::vector<double>> getDots() {
    return {plot_.getVectorX(), plot_.getVectorY()};
  };

 private:
  enum class Lexem : int {
    sin = 1, cos, tan,
    aSin, aCos, aTan,
    sqrt, log, log10,
    deg, mul, div, sum, sub, mod,
    unaryMinus, brOpen,
    num, num_x
  };  // class Lexem

  enum class LType : int {
    num, func, op
  };  // class LType

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

  class Plot {
   public:
    void setPlotLimits(std::vector<double> plot_limits);
    void calculateDots(CalcModel *model);
    double calculateOneDot (double &x_curr, const double &x_prev,
                             const double &acc, CalcModel *model);
    void calculateDeltas(double &delta_x, double &delta_y);
    std::vector<double> getVectorX() { return vector_x_; }
    std::vector<double> getVectorY() { return vector_y_; }
    std::pair<std::vector<double>, std::vector<double>> getDots() {
      return {vector_x_, vector_y_};
    };

   private:
    double x_min_{};
    double x_max_{};
    double y_min_{};
    double y_max_{};
    double x_curr_{};
    double x_prev_{};
    double y_curr_{};
    double y_prev_{};
    std::vector<double> vector_x_{};
    std::vector<double> vector_y_{};
  };  // class Plot

  Plot plot_;
  std::string expression_{};
  std::string result_string_ = "Error";
  double x_value_{};
  double result_{};
  bool expect_unary_operator_{};
  std::vector<Token> postfix_{};
  std::unordered_map<std::string, Lexem> functions_;
  std::unordered_map<char, Lexem> operators_;
  std::unordered_map<Lexem, int> priorities_;
  std::stack<Token> stack_of_operators_;

  void setExpression(const std::string &expression) { expression_ = expression; };
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
  void setAccuracy();
  void trimTrailingZeros();
  void ClearStackOfOperators();

};  // class CalcModel
}  // namespace s21

#endif  // SRC_MODEL_MAIN_MODEL_H_

//void s21::CalcModel::Plot::calculateDots(CalcModel *model) {
//  vector_x_.clear();
//  vector_y_.clear();
//  double step = (x_max_ - x_min_) / 289;
//  double delta_x = 0, delta_y = 0;
//  x_curr_ = x_min_;
//  x_prev_ = x_min_ - step;
//  y_prev_ = y_min_;
//  while (x_curr_ <= x_max_) {
//    x_curr_ = x_curr_ + step;
//    y_curr_ = calculateOneDot(x_curr_, x_curr_, step, model);
//    if (y_curr_ != 0 && y_curr_ >= y_min_ && y_curr_ <= y_max_) {
//      calculateDeltas(delta_x, delta_y);
//      while ((delta_x > 1.5 || delta_y > 1.5) /*&& (delta_x < 20 || delta_y < 20) && (step > 1e-07)*/) {
//        (delta_x > 1.5 || delta_y > 1.5) ? step /= 1.01 : step *= 1.01;
//        y_curr_ = calculateOneDot(x_curr_, x_prev_, step, model);
//        calculateDeltas(delta_x, delta_y);
//      }
//      x_prev_ = x_curr_;
//      y_prev_ = y_curr_;
//      vector_x_.push_back(x_curr_);
//      vector_y_.push_back(y_curr_);
//    }
//  }
//}
//
//void s21::CalcModel::Plot::calculateDeltas(double &delta_x, double &delta_y) {
//  delta_x = x_curr_ - x_prev_;
//  delta_y = x_curr_ - y_prev_;
//}
//
//double s21::CalcModel::Plot::calculateOneDot(double &x_curr,
//                                             const double &x_prev,
//                                             const double &step,
//                                             CalcModel *model) {
//  x_curr = x_prev + step;
//  model->setXValue(x_curr);
//  model->calculateExpression();
//  return model->result_;
//}
