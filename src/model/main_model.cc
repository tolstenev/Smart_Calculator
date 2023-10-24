#include "main_model.h"

#include <utility>

s21::CalculatorModel::CalculatorModel() {
  result_string_ = "Error";
  result_ = std::numeric_limits<double>::infinity();
  InitFunctions();
  InitOperators();
  InitPriorities();
}

void s21::CalculatorModel::InitFunctions() {
  functions_ = {{"X", Lexem::kVariableX}, {"-", Lexem::kUnaryMinus},
                {"sin", Lexem::kSin},     {"cos", Lexem::kCos},
                {"tan", Lexem::kTan},     {"asin", Lexem::kArcSin},
                {"acos", Lexem::kArcCos}, {"atan", Lexem::kArcTan},
                {"sqrt", Lexem::kSqrt},   {"log", Lexem::kLog},
                {"log10", Lexem::kLog10}};
}

void s21::CalculatorModel::InitOperators() {
  operators_ = {{'^', Lexem::kDeg}, {'*', Lexem::kMul}, {'/', Lexem::kDiv},
                {'%', Lexem::kMod}, {'+', Lexem::kSum}, {'-', Lexem::kSub}};
}

void s21::CalculatorModel::InitPriorities() {
  priorities_ = {
      {Lexem::kSin, 0},        {Lexem::kCos, 0},    {Lexem::kTan, 0},
      {Lexem::kArcSin, 0},     {Lexem::kArcCos, 0}, {Lexem::kArcTan, 0},
      {Lexem::kSqrt, 0},       {Lexem::kLog, 0},    {Lexem::kLog10, 0},
      {Lexem::kUnaryMinus, 1}, {Lexem::kDeg, 2},    {Lexem::kMul, 3},
      {Lexem::kDiv, 3},        {Lexem::kMod, 3},    {Lexem::kSum, 4},
      {Lexem::kSub, 4}};
}

void s21::CalculatorModel::Calculate(const std::string &expression) {
  SetExpression(expression);
  try {
    if (IsExpressionValid()) {
      ConvertExpressionToPostfix();
      CalculateExpression();
      ConvertResultToString();
    }
  } catch (...) {
    result_string_ = "Error";
  }
}

bool s21::CalculatorModel::IsExpressionValid() {
  bool valid = false;
  exprtk::symbol_table<double> symbol_table;
  exprtk::expression<double> expression;
  exprtk::parser<double> parser;
  symbol_table.add_constants();
  symbol_table.add_variable("X", x_value_);
  expression.register_symbol_table(symbol_table);
  SubstituteExpression();  // "log" -> "log10", "ln" -> "log",
  // "mod" -> "%", "E" -> "*10^"
  if (parser.compile(expression_, expression)) {
    valid = true;
  };
  return valid;
}

void s21::CalculatorModel::SubstituteExpression() {
  ReplaceInExpression("log", "log10");
  ReplaceInExpression("ln", "log");
  ReplaceInExpression("mod", "%");
  ReplaceInExpression("E", "*10^");
}

void s21::CalculatorModel::ReplaceInExpression(const std::string &from,
                                               const std::string &to) {
  size_t start_index = 0;
  while ((start_index = expression_.find(from, start_index)) !=
         std::string::npos) {
    expression_.replace(start_index, from.length(), to);
    start_index += to.length();
  }
}

void s21::CalculatorModel::ConvertExpressionToPostfix() {
  expect_unary_operator_ = true;
  int which = 0;
  size_t index = 0;
  size_t expression_length = expression_.length();

  postfix_.clear();
  ClearStackOfOperators();

  while (index < expression_length) {
    if (IsNumber(index)) {
      HandleNumber(index);
    } else if (IsX(index)) {
      HandleX(index);
    } else if ((which = IsFunction(index))) {
      HandleFunction(which);
    } else if (IsOpeningBrace(index)) {
      HandleOpeningBrace(index);
    } else if (IsClosingBrace(index)) {
      HandleClosingBrace(index);
    } else {
      HandleOperator(index);
    }
  }
  while (StackOfOperatorsIsNotEmpty()) {
    MoveOperatorsFromStackToVector();
  }
}

void s21::CalculatorModel::ClearStackOfOperators() {
  while (StackOfOperatorsIsNotEmpty()) {
    stack_of_operators_.pop();
  }
}

bool s21::CalculatorModel::StackOfOperatorsIsNotEmpty() {
  return !stack_of_operators_.empty();
}

bool s21::CalculatorModel::IsNumber(size_t index) const {
  return std::isdigit(expression_[index]) || expression_[index] == '.';
}

void s21::CalculatorModel::HandleNumber(size_t &index) {
  postfix_.emplace_back(LexemType::kTypeNumber, Lexem::kNumber,
                        ExtractDigit(index));
  expect_unary_operator_ = false;
}

double s21::CalculatorModel::ExtractDigit(size_t &index) {
  size_t start_index = index;
  while (index < expression_.length() &&
         (std::isdigit(expression_[index]) || expression_[index] == '.'))
    ++index;
  return std::stod(expression_.substr(start_index, index - start_index));
}

bool s21::CalculatorModel::IsX(size_t index) const {
  return expression_[index] == 'X';
}

void s21::CalculatorModel::HandleX(size_t &index) {
  postfix_.emplace_back(LexemType::kTypeNumber, Lexem::kVariableX);
  expect_unary_operator_ = false;
  ++index;
}

int s21::CalculatorModel::IsFunction(size_t &index) {
  if (index + 4 < expression_.length() &&
      expression_.substr(index, 5) == "log10") {
    index += 5;
    return static_cast<int>(Lexem::kLog10);
  }
  for (const auto &function : functions_) {
    const std::string &func_str = function.first;
    if (index + func_str.length() <= expression_.length()) {
      std::string func_candidate = expression_.substr(index, func_str.length());
      if (func_candidate == func_str) {
        if (!expect_unary_operator_ && (func_candidate == "-")) {
          break;
        } else {
          index += func_str.length();
          return static_cast<int>(function.second);
        }
      }
    }
  }
  return 0;
}

void s21::CalculatorModel::HandleFunction(int type_function) {
  stack_of_operators_.emplace(LexemType::kTypeFunction,
                              static_cast<Lexem>(type_function));
  expect_unary_operator_ = false;
}

bool s21::CalculatorModel::IsOpeningBrace(size_t index) const {
  return expression_[index] == '(';
}

void s21::CalculatorModel::HandleOpeningBrace(size_t &index) {
  stack_of_operators_.emplace(LexemType::kTypeOperator, Lexem::kOpenBrace);
  expect_unary_operator_ = true;
  ++index;
}

bool s21::CalculatorModel::IsClosingBrace(size_t index) const {
  return expression_[index] == ')';
}

void s21::CalculatorModel::HandleClosingBrace(size_t &index) {
  ++index;
  while (StackOfOperatorsIsNotEmpty() &&
         stack_of_operators_.top().GetName() != Lexem::kOpenBrace) {
    MoveOperatorsFromStackToVector();
  }
  if (StackOfOperatorsIsNotEmpty() &&
      stack_of_operators_.top().GetName() == Lexem::kOpenBrace) {
    stack_of_operators_.pop();
  }
  expect_unary_operator_ = false;
}

void s21::CalculatorModel::HandleOperator(size_t &index) {
  Lexem handling_operator = CharToLexem(expression_[index++]);
  while (StackOfOperatorsIsNotEmpty() &&
         stack_of_operators_.top().GetName() != Lexem::kOpenBrace &&
         GetPriority(stack_of_operators_.top().GetName()) <=
             GetPriority(handling_operator)) {
    if (handling_operator == Lexem::kDeg &&
        stack_of_operators_.top().GetName() == Lexem::kDeg) {
      break;
    }
    MoveOperatorsFromStackToVector();
  }
  stack_of_operators_.emplace(LexemType::kTypeOperator, handling_operator);
  expect_unary_operator_ = true;
}

s21::CalculatorModel::Lexem s21::CalculatorModel::CharToLexem(
    const char &oper_candidate) {
  return operators_.at(oper_candidate);
}

int s21::CalculatorModel::GetPriority(const Lexem &lexem) {
  return priorities_.at(lexem);
}

void s21::CalculatorModel::MoveOperatorsFromStackToVector() {
  postfix_.push_back(stack_of_operators_.top());
  stack_of_operators_.pop();
}

void s21::CalculatorModel::CalculateExpression() {
  if (!postfix_.empty()) {
    CalculatePostfix();
  }
}

void s21::CalculatorModel::CalculatePostfix() {
  std::stack<double> numbers;
  for (size_t index = 0; index < postfix_.size(); ++index) {
    Token current = postfix_[index];
    if (IsNumericToken(current)) {
      HandleNumericToken(current, numbers);
    } else if (IsFunctionToken(current)) {
      HandleFunctionToken(current, numbers);
    } else if (IsOperationToken(current)) {
      HandleOperationToken(current, numbers);
    }
  }
  if (numbers.size() == 1) {
    result_ = numbers.top();
  }
}

bool s21::CalculatorModel::IsNumericToken(const Token &token) {
  return token.GetType() == LexemType::kTypeNumber;
}

void s21::CalculatorModel::HandleNumericToken(const Token &token,
                                              std::stack<double> &numbers) {
  if (IsXToken(token)) {
    numbers.push(x_value_);
  } else {
    numbers.push(token.GetValue());
  }
}

bool s21::CalculatorModel::IsXToken(const Token &token) {
  return token.GetName() == Lexem::kVariableX;
}

bool s21::CalculatorModel::IsFunctionToken(const Token &token) {
  return token.GetType() == LexemType::kTypeFunction;
}

bool s21::CalculatorModel::IsOperationToken(const Token &token) {
  return token.GetType() == LexemType::kTypeOperator;
}

void s21::CalculatorModel::HandleFunctionToken(const Token &token,
                                               std::stack<double> &numbers) {
  double num = numbers.top();
  numbers.pop();

  double result;
  switch (token.GetName()) {
    case Lexem::kUnaryMinus:
      result = -1 * num;
      break;
    case Lexem::kSqrt:
      result = std::sqrt(num);
      break;
    case Lexem::kLog10:
      result = std::log10(num);
      break;
    case Lexem::kLog:
      result = std::log(num);
      break;
    case Lexem::kSin:
      result = std::sin(num);
      break;
    case Lexem::kCos:
      result = std::cos(num);
      break;
    case Lexem::kTan:
      result = std::tan(num);
      break;
    case Lexem::kArcSin:
      result = std::asin(num);
      break;
    case Lexem::kArcCos:
      result = std::acos(num);
      break;
    default: /*Lexem::kArcTan*/
      result = std::atan(num);
      break;
  }
  numbers.push(result);
}

void s21::CalculatorModel::HandleOperationToken(const Token &token,
                                                std::stack<double> &numbers) {
  if (numbers.size() >= 2) {
    double rhs = numbers.top();
    numbers.pop();
    double lhs = numbers.top();
    numbers.pop();

    double result;
    switch (token.GetName()) {
      case Lexem::kSum:
        result = lhs + rhs;
        break;
      case Lexem::kSub:
        result = lhs - rhs;
        break;
      case Lexem::kMul:
        result = lhs * rhs;
        break;
      case Lexem::kDiv:
        result = lhs / rhs;
        break;
      case Lexem::kMod:
        result = std::fmod(lhs, rhs);
        break;
      default: /*Lexem::kDeg*/
        result = std::pow(lhs, rhs);
        break;
    }
    numbers.push(result);
  }
}

void s21::CalculatorModel::ConvertResultToString() {
  if (IsResultError()) {
    result_string_ = "Error";
  } else {
    SetAccuracy();
    TrimTrailingZeros();
  }
}

bool s21::CalculatorModel::IsResultError() const {
  return std::isinf(result_) || std::isnan(result_);
}

void s21::CalculatorModel::SetAccuracy() {
  std::ostringstream stream;
  stream.precision(8);
  stream << std::fixed << result_;
  result_string_ = stream.str();
}

void s21::CalculatorModel::TrimTrailingZeros() {
  size_t iter = result_string_.find_last_not_of('0');
  if (result_string_[iter] == '.') {
    --iter;
  }
  result_string_ = result_string_.substr(0, ++iter);
}

void s21::CalculatorModel::CalculateDots(const std::string &expression,
                                         std::vector<double> plot_limits) {
  plot_.SetPlotLimits(std::move(plot_limits));
  Calculate(expression);
  plot_.CalculateDots(this);
}

void s21::CalculatorModel::Plot::SetPlotLimits(
    std::vector<double> plot_limits) {
  x_min_ = plot_limits[0];
  x_max_ = plot_limits[1];
  y_min_ = plot_limits[2];
  y_max_ = plot_limits[3];
}

void s21::CalculatorModel::Plot::CalculateDots(CalculatorModel *model) {
  InitializeData();
  while (x_curr_ <= x_max_) {
    AdjustDot(model);
    UpdatePreviousValues();
    AddDotToList();
  }
}

void s21::CalculatorModel::Plot::InitializeData() {
  list_x_.clear();
  list_y_.clear();
  x_step_ = (x_max_ - x_min_) / 290;
  y_step_ = (y_max_ - y_min_) / 250;
  x_curr_ = x_min_;
  x_prev_ = x_min_ - x_step_;
  y_prev_ = y_min_;
}

void s21::CalculatorModel::Plot::AdjustDot(CalculatorModel *model) {
  CalculateOneDot(x_curr_, model);
  if (IsValidDot()) {
    CalculateDeltas();

    while (delta_x_ > x_step_ && delta_y_ > y_step_) {
      if (delta_x_ > x_step_ || delta_y_ > y_step_) {
        x_step_ /= 1.01;
      }
      CalculateOneDot(x_prev_, model);
      CalculateDeltas();
    }
  }
}

void s21::CalculatorModel::Plot::CalculateOneDot(const double &x_prev,
                                                 CalculatorModel *model) {
  x_curr_ = x_prev + x_step_;
  model->SetXValue(x_curr_);
  model->CalculateExpression();
  y_curr_ = model->result_;
}

bool s21::CalculatorModel::Plot::IsValidDot() const {
  return (y_curr_ != 0 && y_curr_ >= y_min_ && y_curr_ <= y_max_);
}

void s21::CalculatorModel::Plot::CalculateDeltas() {
  delta_x_ = x_curr_ - x_prev_;
  delta_y_ = x_curr_ - y_prev_;
}

void s21::CalculatorModel::Plot::UpdatePreviousValues() {
  x_prev_ = x_curr_;
  y_prev_ = y_curr_;
}

void s21::CalculatorModel::Plot::AddDotToList() {
  list_x_.emplace_back(x_curr_);
  list_y_.emplace_back(y_curr_);
}
