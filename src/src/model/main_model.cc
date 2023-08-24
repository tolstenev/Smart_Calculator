#include "main_model.h"

s21::CalcModel::CalcModel() {
    initUnaryLexems();
    initBinaryLexems();
    initPriorities();
}

void s21::CalcModel::initUnaryLexems() {
    functions_ = {
            {"sin",   Lexem::sin},
            {"cos",   Lexem::cos},
            {"tan",   Lexem::tan},
            {"asin",  Lexem::aSin},
            {"acos",  Lexem::aCos},
            {"atan",  Lexem::aTan},
            {"sqrt",  Lexem::sqrt},
            {"log",   Lexem::log},
            {"log10", Lexem::log10}};
}

void s21::CalcModel::initBinaryLexems() {
    operators_ = {
            {'^', Lexem::deg},
            {'*', Lexem::mul},
            {'/', Lexem::div},
            {'%', Lexem::mod},
            {'+', Lexem::sum},
            {'-', Lexem::sub}};
}

void s21::CalcModel::initPriorities() {
    priorities_ = {
            {Lexem::sin,        0},
            {Lexem::cos,        0},
            {Lexem::tan,        0},
            {Lexem::aSin,       0},
            {Lexem::aCos,       0},
            {Lexem::aTan,       0},
            {Lexem::sqrt,       0},
            {Lexem::log,        0},
            {Lexem::log10,      0},
            {Lexem::unaryMinus, 1},
            {Lexem::deg,        2},
            {Lexem::mul,        3},
            {Lexem::div,        3},
            {Lexem::mod,        3},
            {Lexem::sum,        4},
            {Lexem::sub,        4}};
}

void s21::CalcModel::validateExpression() {
    exprtk::symbol_table<double> symbol_table;
    exprtk::expression<double> expression;
    exprtk::parser<double> parser;
    symbol_table.add_constants();
    symbol_table.add_variable("X", x_value_);
    expression.register_symbol_table(symbol_table);
    substituteExpression();  // "log" -> "log10", "ln" -> "log", "mod" -> "%", "E" -> "*10^"
    parser.compile(expr_, expression);
}

void s21::CalcModel::substituteExpression() {
    replaceInExpression("log", "log10");
    replaceInExpression("ln", "log");
    replaceInExpression("mod", "%");
    replaceInExpression("E", "*10^");
}

void s21::CalcModel::replaceInExpression(const std::string &from, const std::string &to) {
    size_t startIndex = 0;
    while ((startIndex = expr_.find(from, startIndex)) != std::string::npos) {
        expr_.replace(startIndex, from.length(), to);
        startIndex += to.length();
    }
}

double s21::CalcModel::extractDigit(size_t &pos) {
    size_t start = pos;
    while (pos < expr_.length() &&
           (std::isdigit(expr_[pos]) || expr_[pos] == '.'))
        ++pos;
    return std::stod(expr_.substr(start, pos - start));
}

void s21::CalcModel::calculateExpression() {
    if (!postfix_.empty()) {
        calculatePostfix();
    }
//    convertResultToString();
}

int s21::CalcModel::getPriority(const Lexem &lexem) {
    return priorities_.at(lexem);
}

void s21::CalcModel::convertExpressionToPostfix() {
    std::stack<Token> operators;
    bool is_unary = true;
    int which = 0;
    size_t index = 0;
    size_t expression_length = expr_.length();
    postfix_.clear();
    while (index < expression_length) {
        if (isUnaryOperator(index, is_unary)) {
            handleUnaryOperator(index, operators);
        } else if (isNumeric(index)) {
            handleNumeric(index, is_unary);
        } else if (isVariable(index)) {
            handleVariable(index, is_unary);
        } else if ((which = isFunction(index))) {
            handleFunction(which, operators, is_unary);
        } else if (isOpeningBrace(index)) {
            handleOpeningBrace(index, operators, is_unary);
        } else if (isClosingBrace(index)) {
            handleClosingBrace(index, operators, is_unary);
        } else {
            handleOperator(index, operators, is_unary);
        }
    }
    while (!operators.empty()) {
        stackToVector(operators);
    }
}

bool s21::CalcModel::isUnaryOperator(size_t index, bool &is_unary) const {
    return is_unary && (expr_[index] == '-' || expr_[index] == '+');
}

bool s21::CalcModel::isVariable(size_t index) const {
    return expr_[index] == 'x';
}

int s21::CalcModel::isFunction(size_t &index) const {
    if (index + 4 < expr_.length() && expr_.substr(index, index + 5) == "log10") {
        index += 5;
        return static_cast<int>(Lexem::log10);
    }
    for (const auto &function: functions_) {
        const std::string &func_str = function.first;
        if (index + func_str.length() <= expr_.length()) {
            if (expr_.substr(index, func_str.length()) == func_str) {
                index += func_str.length();
                return static_cast<int>(function.second);
            }
        }
    }
    return 0;
}

bool s21::CalcModel::isOpeningBrace(size_t index) const {
    return expr_[index] == '(';
}

bool s21::CalcModel::isClosingBrace(size_t index) const {
    return expr_[index] == ')';
}

bool s21::CalcModel::isNumeric(size_t index) const {
    return std::isdigit(expr_[index]) || expr_[index] == '.';
}

void s21::CalcModel::handleUnaryOperator(size_t &index, std::stack<Token> &operators) {
    if (expr_[index] == '-') {
        operators.emplace(LType::op, Lexem::unaryMinus);
    }
    ++index;
}

void s21::CalcModel::handleVariable(size_t &index, bool &is_unary) {
    postfix_.emplace_back(LType::num, Lexem::num_x);
    is_unary = false;
    ++index;
}

void s21::CalcModel::handleFunction(int func_type, std::stack<Token> &operators, bool &is_unary) {
    operators.emplace(LType::func, static_cast<Lexem>(func_type));
    is_unary = false;
}

void s21::CalcModel::handleOpeningBrace(size_t &index, std::stack<Token> &operators, bool &is_unary) {
    operators.emplace(LType::op, Lexem::brOpen);
    is_unary = true;
    ++index;
}

void s21::CalcModel::handleClosingBrace(size_t &index, std::stack<Token> &operators, bool &is_unary) {
    ++index;
    while (!operators.empty() && operators.top().getName() != Lexem::brOpen) {
        stackToVector(operators);
    }
    if (!operators.empty() && operators.top().getName() == Lexem::brOpen) {
        operators.pop();
    }
    is_unary = false;
}

void s21::CalcModel::handleNumeric(size_t &index, bool &is_unary) {
    postfix_.emplace_back(LType::num, Lexem::num, extractDigit(index));
    is_unary = false;
}

void s21::CalcModel::handleOperator(size_t &index, std::stack<Token> &operators, bool &is_unary) {
    Lexem handlingOperator = charToLexem(expr_[index++]);
    while (!operators.empty() &&
           operators.top().getName() != Lexem::brOpen &&
           getPriority(operators.top().getName()) <= getPriority(handlingOperator)) {
        if (handlingOperator == Lexem::deg && operators.top().getName() == Lexem::deg) {
            break;
        }
        stackToVector(operators);
    }
    operators.emplace(LType::op, handlingOperator);
    is_unary = true;
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

void s21::CalcModel::stackToVector(std::stack<Token> &operators) {
    postfix_.push_back(operators.top());
    operators.pop();
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
        throw std::runtime_error("Calculation fail");
    }
}

bool s21::CalcModel::isNumber(const Token& token) {
    return token.getType() == LType::num;
}

bool s21::CalcModel::isFunction(const Token& token) {
    return token.getType() == LType::func;
}

bool s21::CalcModel::isOperation(const Token& token) {
    return token.getType() == LType::op;
}

void s21::CalcModel::handleNumber(const Token& token, std::stack<double> &numbers) {
    isX(token) ? numbers.push(x_value_)
               : numbers.push(token.getValue());
}

bool s21::CalcModel::isX(const Token& token) {
    return token.getName() == Lexem::num_x;
}

void s21::CalcModel::handleFunction(const Token& token, std::stack<double> &numbers) {
    double num = numbers.top();
    numbers.pop();

    double result;
    switch (token.getName()) {
        case Lexem::sqrt:
            result = std::sqrt(num);
            break;
        case Lexem::log:
            result = std::log(num);
            break;
        case Lexem::log10:
            result = std::log10(num);
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
        default:  // Lexem::aTan
            result = std::atan(num);
            break;
    }

    numbers.push(result);
}

void s21::CalcModel::handleOperation(const Token& token, std::stack<double> &numbers) {
    if (numbers.size() < 2) {
        throw std::runtime_error("Calculation fail");
    }

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
        default:  // Lexem::deg
            result = std::pow(lhs, rhs);
            break;
    }

    numbers.push(result);
}
