#ifndef SRC_MODEL_MAIN_MODEL_H_
#define SRC_MODEL_MAIN_MODEL_H_

#include <sstream>
#include <list>
#include <string>
#include <unordered_map>
#include "../rcs/exprtk.hpp"

namespace s21 {
class CalculatorModel {
 public:
  CalculatorModel();

  void SetXValue(double x) { x_value_ = x; };
  void Calculate(const std::string &expression);
  std::string GetResultString() { return result_string_; };
  void CalculateDots(const std::string &expression, std::vector<double> plot_limits);
  std::pair<std::list<double>, std::list<double>> GetDots() {
    return {plot_.GetListX(), plot_.GetListY() };
  };

 private:
  enum class Lexem : int {
    kSin = 1, kCos, kTan,
    kArcSin, kArcCos, kArcTan,
    kSqrt, kLog, kLog10,
    kDeg, kMul, kDiv, kSum, kSub, kMod,
    kUnaryMinus, kOpenBrace,
    kNumber, kVariableX
  };  // class Lexem

  enum class LexemType : int {
    kTypeNumber, kTypeFunction, kTypeOperator
  };  // class LexemType

  class Token {
   public:
    Token(LexemType type, Lexem name)
        : type_(type), name_(name), value_{} {}
    Token(LexemType type, Lexem name, double value)
        : type_(type), name_(name), value_(value) {}
    LexemType GetType() const { return type_; }
    Lexem GetName() const { return name_; }
    double GetValue() const { return value_; }

   private:
    LexemType type_{};
    Lexem name_{};
    double value_{};
  };  // class Token

  class Plot {
   public:
    void SetPlotLimits(std::vector<double> plot_limits);
    void CalculateDots(CalculatorModel *model);
    void InitializeData();
    void AdjustDot(CalculatorModel *model);
    void CalculateOneDot (const double &x_prev, CalculatorModel *model);
    bool IsValidDot() const;
    void CalculateDeltas();
    void UpdatePreviousValues();
    void AddDotToList();

    std::list<double> GetListX() { return list_x_; }
    std::list<double> GetListY() { return list_y_; }

   private:
    double x_min_{};
    double x_max_{};
    double y_min_{};
    double y_max_{};
    double x_curr_{};
    double x_prev_{};
    double y_curr_{};
    double y_prev_{};
    double x_step_{};
    double y_step_{};
    double delta_x_{};
    double delta_y_{};
    std::list<double> list_x_{};
    std::list<double> list_y_{};
  };  // class Plot

  Plot plot_;
  std::string expression_{};
  std::string result_string_ = "Error";
  double result_ = std::numeric_limits<double>::infinity();
  double x_value_{};
  bool expect_unary_operator_{};
  std::vector<Token> postfix_{};
  std::unordered_map<std::string, Lexem> functions_;
  std::unordered_map<char, Lexem> operators_;
  std::unordered_map<Lexem, int> priorities_;
  std::stack<Token> stack_of_operators_;

  void InitFunctions();
  void InitOperators();
  void InitPriorities();

  void SetExpression(const std::string &expression) { expression_ = expression; };
  bool IsExpressionValid();
  void SubstituteExpression();
  void ReplaceInExpression(const std::string &from, const std::string &to);
  void ConvertExpressionToPostfix();
  void ClearStackOfOperators();

  bool IsNumber(size_t index) const;
  bool IsX(size_t index) const;
  int IsFunction(size_t &index);
  bool IsOpeningBrace(size_t index) const;
  bool IsClosingBrace(size_t index) const;

  void HandleX(size_t &index);
  void HandleFunction(int type_function);
  void HandleOpeningBrace(size_t &index);
  void HandleClosingBrace(size_t &index);
  void HandleNumber(size_t &index);
  void HandleOperator(size_t &index);

  double ExtractDigit(size_t &pos);
  Lexem CharToLexem(const char &oper);
  int GetPriority(const Lexem &lexem);

  void StackOfOperatorsToVector();
  void CalculateExpression();
  void CalculatePostfix();

  bool IsNumericToken(const Token &token);
  bool IsXToken(const Token &token);
  bool IsFunctionToken(const Token &token);
  bool IsOperationToken(const Token &token);

  void HandleNumericToken(const Token &token, std::stack<double> &numbers);
  void HandleFunctionToken(const Token &token, std::stack<double> &numbers);
  void HandleOperationToken(const Token &token, std::stack<double> &numbers);

  void ConvertResultToString();
  bool IsResultError() const;
  void SetAccuracy();
  void TrimTrailingZeros();

};  // class CalculatorModel
}  // namespace s21

#endif  // SRC_MODEL_MAIN_MODEL_H_