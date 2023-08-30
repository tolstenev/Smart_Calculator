#include "main_model.h"



s21::CalcModel::CalcModel() {
  initFunctions();
  initOperators();
  initPriorities();
}

void s21::CalcModel::initFunctions() {
  functions_ = {
      {"X", Lexem::num_x},   {"-", Lexem::unaryMinus}, {"sin", Lexem::sin},
      {"cos", Lexem::cos},   {"tan", Lexem::tan},      {"asin", Lexem::aSin},
      {"acos", Lexem::aCos}, {"atan", Lexem::aTan},    {"sqrt", Lexem::sqrt},
      {"log", Lexem::log},   {"log10", Lexem::log10}};
}

void s21::CalcModel::initOperators() {
  operators_ = {{'^', Lexem::deg}, {'*', Lexem::mul}, {'/', Lexem::div},
                {'%', Lexem::mod}, {'+', Lexem::sum}, {'-', Lexem::sub}};
}

void s21::CalcModel::initPriorities() {
  priorities_ = {{Lexem::sin, 0},        {Lexem::cos, 0},  {Lexem::tan, 0},
                 {Lexem::aSin, 0},       {Lexem::aCos, 0}, {Lexem::aTan, 0},
                 {Lexem::sqrt, 0},       {Lexem::log, 0},  {Lexem::log10, 0},
                 {Lexem::unaryMinus, 1}, {Lexem::deg, 2},  {Lexem::mul, 3},
                 {Lexem::div, 3},        {Lexem::mod, 3},  {Lexem::sum, 4},
                 {Lexem::sub, 4}};
}

bool s21::CalcModel::isExpressionValid() {
  bool valid = false;
  exprtk::symbol_table<double> symbol_table;
  exprtk::expression<double> expression;
  exprtk::parser<double> parser;
  symbol_table.add_constants();
  symbol_table.add_variable("X", x_value_);
  expression.register_symbol_table(symbol_table);
  substituteExpression();  // "log" -> "log10", "ln" -> "log",
  // "mod" -> "%", "E" -> "*10^"
  if (parser.compile(expression_, expression)) {
    valid = true;
  };
  return valid;
}

void s21::CalcModel::substituteExpression() {
  replaceInExpression("log", "log10");
  replaceInExpression("ln", "log");
  replaceInExpression("mod", "%");
  replaceInExpression("E", "*10^");
}

void s21::CalcModel::replaceInExpression(const std::string &from,
                                         const std::string &to) {
  size_t start_index = 0;
  while ((start_index = expression_.find(from, start_index)) != std::string::npos) {
    expression_.replace(start_index, from.length(), to);
    start_index += to.length();
  }
}

double s21::CalcModel::extractDigit(size_t &pos) {
  size_t start = pos;
  while (pos < expression_.length() &&
         (std::isdigit(expression_[pos]) || expression_[pos] == '.'))
    ++pos;
  return std::stod(expression_.substr(start, pos - start));
}

void s21::CalcModel::calculateExpression() {
  if (!postfix_.empty()) {
    calculatePostfix();
  }
}

void s21::CalcModel::convertResultToString() {
  if (isResultError()) {
    result_string_ = "Error";
  } else {
    setAccuracy();
    trimTrailingZeros();
  }
}

bool s21::CalcModel::isResultError() const {
  return std::isinf(result_) || std::isnan(result_);
}

void s21::CalcModel::setAccuracy() {
  std::ostringstream stream;
  stream.precision(8);
  stream << std::fixed << result_;
  result_string_ = stream.str();
}

void s21::CalcModel::trimTrailingZeros() {
  size_t iter = result_string_.find_last_not_of('0');
  if (result_string_[iter] == '.') {
    --iter;
  }
  result_string_ = result_string_.substr(0, ++iter);
}

int s21::CalcModel::getPriority(const Lexem &lexem) {
  return priorities_.at(lexem);
}

void s21::CalcModel::convertExpressionToPostfix() {
  expect_unary_operator_ = true;
  int which = 0;
  size_t index = 0;
  size_t expression_length = expression_.length();

  postfix_.clear();
  ClearStackOfOperators();

  while (index < expression_length) {
    if (isNumeric(index)) {
      handleNumeric(index);
    } else if (isVariable(index)) {
      handleVariable(index);
    } else if ((which = isFunction(index))) {
      handleFunction(which);
    } else if (isOpeningBrace(index)) {
      handleOpeningBrace(index);
    } else if (isClosingBrace(index)) {
      handleClosingBrace(index);
    } else {
      handleOperator(index);
    }
  }
  while (!stack_of_operators_.empty()) {
    stackOfOperatorsToVector();
  }
}

void s21::CalcModel::ClearStackOfOperators() {
  while (!stack_of_operators_.empty()) {
    stack_of_operators_.pop();
  }
}

bool s21::CalcModel::isVariable(size_t index) const {
  return expression_[index] == 'X';
}

int s21::CalcModel::isFunction(size_t &index) {
  if (index + 4 < expression_.length() && expression_.substr(index, 5) == "log10") {
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

bool s21::CalcModel::isOpeningBrace(size_t index) const {
  return expression_[index] == '(';
}

bool s21::CalcModel::isClosingBrace(size_t index) const {
  return expression_[index] == ')';
}

bool s21::CalcModel::isNumeric(size_t index) const {
  return std::isdigit(expression_[index]) || expression_[index] == '.';
}

void s21::CalcModel::handleVariable(size_t &index) {
  postfix_.emplace_back(LType::num, Lexem::num_x);
  expect_unary_operator_ = false;
  ++index;
}

void s21::CalcModel::handleFunction(int func_type) {
  stack_of_operators_.emplace(LType::func, static_cast<Lexem>(func_type));
  expect_unary_operator_ = false;
}

void s21::CalcModel::handleOpeningBrace(size_t &index) {
  stack_of_operators_.emplace(LType::op, Lexem::brOpen);
  expect_unary_operator_ = true;
  ++index;
}

void s21::CalcModel::handleClosingBrace(size_t &index) {
  ++index;
  while (!stack_of_operators_.empty() &&
         stack_of_operators_.top().getName() != Lexem::brOpen) {
    stackOfOperatorsToVector();
  }
  if (!stack_of_operators_.empty() &&
      stack_of_operators_.top().getName() == Lexem::brOpen) {
    stack_of_operators_.pop();
  }
  expect_unary_operator_ = false;
}

void s21::CalcModel::handleNumeric(size_t &index) {
  postfix_.emplace_back(LType::num, Lexem::num, extractDigit(index));
  expect_unary_operator_ = false;
}

void s21::CalcModel::handleOperator(size_t &index) {
  Lexem handlingOperator = charToLexem(expression_[index++]);
  while (!stack_of_operators_.empty() &&
         stack_of_operators_.top().getName() != Lexem::brOpen &&
         getPriority(stack_of_operators_.top().getName()) <=
             getPriority(handlingOperator)) {
    if (handlingOperator == Lexem::deg &&
        stack_of_operators_.top().getName() == Lexem::deg) {
      break;
    }
    stackOfOperatorsToVector();
  }
  stack_of_operators_.emplace(LType::op, handlingOperator);
  expect_unary_operator_ = true;
}

s21::CalcModel::Lexem s21::CalcModel::charToLexem(const char &oper) {
  Lexem lex;
  try {
    lex = operators_.at(oper);
  } catch (const std::out_of_range &e) {
    lex = Lexem::brOpen;
    return lex;
  }
  return lex;
}

void s21::CalcModel::stackOfOperatorsToVector() {
  postfix_.push_back(stack_of_operators_.top());
  stack_of_operators_.pop();
}

void s21::CalcModel::calculatePostfix() {
  std::stack<double> numbers;
  for (size_t index = 0; index < postfix_.size(); ++index) {
    Token currentToken = postfix_[index];
    if (isNumber(currentToken)) {
      handleNumber(currentToken, numbers);
    } else if (isFunction(currentToken)) {
      handleFunction(currentToken, numbers);
    } else if (isOperation(currentToken)) {
      handleOperation(currentToken, numbers);
    }
  }
  if (numbers.size() == 1) {
    result_ = numbers.top();
  } else {
    // Calculation error. Set NaN to result_
    // that will be handle in isResultError()
    result_ = std::numeric_limits<double>::quiet_NaN();
  }
}

bool s21::CalcModel::isNumber(const Token &token) {
  return token.getType() == LType::num;
}

bool s21::CalcModel::isFunction(const Token &token) {
  return token.getType() == LType::func;
}

bool s21::CalcModel::isOperation(const Token &token) {
  return token.getType() == LType::op;
}

void s21::CalcModel::handleNumber(const Token &token,
                                  std::stack<double> &numbers) {
  isX(token) ? numbers.push(x_value_) : numbers.push(token.getValue());
}

bool s21::CalcModel::isX(const Token &token) {
  return token.getName() == Lexem::num_x;
}

void s21::CalcModel::handleFunction(const Token &token,
                                    std::stack<double> &numbers) {
  double num = numbers.top();
  numbers.pop();

  double result;
  switch (token.getName()) {
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

void s21::CalcModel::handleOperation(const Token &token,
                                     std::stack<double> &numbers) {
  if (numbers.size() < 2) {
    // Calculation error. Set NaN to result_ that will be handle in
    // isResultError()
    result_ = std::numeric_limits<double>::quiet_NaN();
  } else {
    double rhs = numbers.top();
    numbers.pop();
    double lhs = numbers.top();
    numbers.pop();

    double result;
    switch (token.getName()) {
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

void s21::CalcModel::calculate(const std::string &expression) {
  setExpression(expression);
  try {
    if (isExpressionValid()) {
      convertExpressionToPostfix();
      calculateExpression();
      convertResultToString();
    }
  } catch (...) {
    result_string_ = "Error";
  }
}

void s21::CalcModel::Plot::setPlotLimits(std::vector<double> plot_limits) {
  x_min_ = plot_limits[0];
  x_max_ = plot_limits[1];
  y_min_ = plot_limits[2];
  y_max_ = plot_limits[3];
}

void s21::CalcModel::Plot::calculateDots(CalcModel *model) {
  vector_x_.clear();
  vector_y_.clear();
  double y_current = 0.0;
  double x_current = 0.0;
  double step = 1.0;
  int steps = static_cast<int>((x_max_ - x_min_) / step);
  for (int i = 0; i <= steps; ++i) {
    x_current = x_min_ + i * step;
    model->calculateExpression();
    y_current = model->result_;
    vector_x_.push_back(x_current);  // переделать на лист
    vector_y_.push_back(y_current);
  }
}
