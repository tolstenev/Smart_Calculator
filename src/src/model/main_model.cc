#include "main_model.h"

#include <utility>

s21::CalculatorModel::CalculatorModel() {
  InitFunctions();
  InitOperators();
  InitPriorities();
}

void s21::CalculatorModel::InitFunctions() {
  functions_ = {
      {"X", Lexem::num_x},   {"-", Lexem::unaryMinus}, {"sin", Lexem::sin},
      {"cos", Lexem::cos},   {"tan", Lexem::tan},      {"asin", Lexem::aSin},
      {"acos", Lexem::aCos}, {"atan", Lexem::aTan},    {"sqrt", Lexem::sqrt},
      {"log", Lexem::log},   {"log10", Lexem::log10}};
}

void s21::CalculatorModel::InitOperators() {
  operators_ = {{'^', Lexem::deg}, {'*', Lexem::mul}, {'/', Lexem::div},
                {'%', Lexem::mod}, {'+', Lexem::sum}, {'-', Lexem::sub}};
}

void s21::CalculatorModel::InitPriorities() {
  priorities_ = {{Lexem::sin, 0},        {Lexem::cos, 0},  {Lexem::tan, 0},
                 {Lexem::aSin, 0},       {Lexem::aCos, 0}, {Lexem::aTan, 0},
                 {Lexem::sqrt, 0},       {Lexem::log, 0},  {Lexem::log10, 0},
                 {Lexem::unaryMinus, 1}, {Lexem::deg, 2},  {Lexem::mul, 3},
                 {Lexem::div, 3},        {Lexem::mod, 3},  {Lexem::sum, 4},
                 {Lexem::sub, 4}};
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
  while (!stack_of_operators_.empty()) {
    StackOfOperatorsToVector();
  }
}

void s21::CalculatorModel::ClearStackOfOperators() {
  while (!stack_of_operators_.empty()) {
    stack_of_operators_.pop();
  }
}

bool s21::CalculatorModel::IsNumber(size_t index) const {
  return std::isdigit(expression_[index]) || expression_[index] == '.';
}

void s21::CalculatorModel::HandleNumber(size_t &index) {
  postfix_.emplace_back(LexemType::num, Lexem::num, ExtractDigit(index));
  expect_unary_operator_ = false;
}

double s21::CalculatorModel::ExtractDigit(size_t &pos) {
  size_t start = pos;
  while (pos < expression_.length() &&
         (std::isdigit(expression_[pos]) || expression_[pos] == '.'))
    ++pos;
  return std::stod(expression_.substr(start, pos - start));
}

bool s21::CalculatorModel::IsX(size_t index) const {
  return expression_[index] == 'X';
}

void s21::CalculatorModel::HandleX(size_t &index) {
  postfix_.emplace_back(LexemType::num, Lexem::num_x);
  expect_unary_operator_ = false;
  ++index;
}

int s21::CalculatorModel::IsFunction(size_t &index) {
  if (index + 4 < expression_.length() &&
      expression_.substr(index, 5) == "log10") {
    index += 5;
    return static_cast<int>(Lexem::log10);
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

void s21::CalculatorModel::HandleFunction(int func_type) {
  stack_of_operators_.emplace(LexemType::func, static_cast<Lexem>(func_type));
  expect_unary_operator_ = false;
}

bool s21::CalculatorModel::IsOpeningBrace(size_t index) const {
  return expression_[index] == '(';
}

void s21::CalculatorModel::HandleOpeningBrace(size_t &index) {
  stack_of_operators_.emplace(LexemType::op, Lexem::brOpen);
  expect_unary_operator_ = true;
  ++index;
}

bool s21::CalculatorModel::IsClosingBrace(size_t index) const {
  return expression_[index] == ')';
}

void s21::CalculatorModel::HandleClosingBrace(size_t &index) {
  ++index;
  while (!stack_of_operators_.empty() &&
         stack_of_operators_.top().GetName() != Lexem::brOpen) {
    StackOfOperatorsToVector();
  }
  if (!stack_of_operators_.empty() &&
      stack_of_operators_.top().GetName() == Lexem::brOpen) {
    stack_of_operators_.pop();
  }
  expect_unary_operator_ = false;
}

void s21::CalculatorModel::HandleOperator(size_t &index) {
  Lexem handling_operator = CharToLexem(expression_[index++]);
  while (!stack_of_operators_.empty() &&
         stack_of_operators_.top().GetName() != Lexem::brOpen &&
         GetPriority(stack_of_operators_.top().GetName()) <=
             GetPriority(handling_operator)) {
    if (handling_operator == Lexem::deg &&
        stack_of_operators_.top().GetName() == Lexem::deg) {
      break;
    }
    StackOfOperatorsToVector();
  }
  stack_of_operators_.emplace(LexemType::op, handling_operator);
  expect_unary_operator_ = true;
}

s21::CalculatorModel::Lexem s21::CalculatorModel::CharToLexem(
    const char &oper) {
  Lexem lex;
  try {
    lex = operators_.at(oper);
  } catch (const std::out_of_range &e) {
    lex = Lexem::brOpen;
    return lex;
  }
  return lex;
}

int s21::CalculatorModel::GetPriority(const Lexem &lexem) {
  return priorities_.at(lexem);
}

void s21::CalculatorModel::StackOfOperatorsToVector() {
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
  } else {
    // Calculation error. Set NaN to result_
    // that will be handle in IsResultError()
    result_ = std::numeric_limits<double>::quiet_NaN();
  }
}

bool s21::CalculatorModel::IsNumericToken(const Token &token) {
  return token.GetType() == LexemType::num;
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
  return token.GetName() == Lexem::num_x;
}

bool s21::CalculatorModel::IsFunctionToken(const Token &token) {
  return token.GetType() == LexemType::func;
}

bool s21::CalculatorModel::IsOperationToken(const Token &token) {
  return token.GetType() == LexemType::op;
}

void s21::CalculatorModel::HandleFunctionToken(const Token &token,
                                               std::stack<double> &numbers) {
  double num = numbers.top();
  numbers.pop();

  double result;
  switch (token.GetName()) {
    case Lexem::unaryMinus:
      result = -1 * num;
      break;
    case Lexem::sqrt:
      result = std::sqrt(num);
      break;
    case Lexem::log10:
      result = std::log10(num);
      break;
    case Lexem::log:
      result = std::log(num);
      break;
    case Lexem::sin:
      result = std::sin(num);
      break;
    case Lexem::cos:
      result = std::cos(num);
      break;
    case Lexem::tan:
      result = std::tan(num);
      break;
    case Lexem::aSin:
      result = std::asin(num);
      break;
    case Lexem::aCos:
      result = std::acos(num);
      break;
    default: /*Lexem::aTan*/
      result = std::atan(num);
      break;
  }
  numbers.push(result);
}

void s21::CalculatorModel::HandleOperationToken(const Token &token,
                                                std::stack<double> &numbers) {
  if (numbers.size() < 2) {
    // Calculation error. Set NaN to result_ that will be handle in
    // IsResultError()
    result_ = std::numeric_limits<double>::quiet_NaN();
  } else {
    double rhs = numbers.top();
    numbers.pop();
    double lhs = numbers.top();
    numbers.pop();

    double result;
    switch (token.GetName()) {
      case Lexem::sum:
        result = lhs + rhs;
        break;
      case Lexem::sub:
        result = lhs - rhs;
        break;
      case Lexem::mul:
        result = lhs * rhs;
        break;
      case Lexem::div:
        result = lhs / rhs;
        break;
      case Lexem::mod:
        result = std::fmod(lhs, rhs);
        break;
      default: /*Lexem::deg*/
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
      } else {
        x_step_ *= 1.01;
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
