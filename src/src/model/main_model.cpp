#include "main_model.h"

s21::CalcModel::CalcModel() {
    initFunctions();
    initOperators();
    initPriorities();
}

void s21::CalcModel::initFunctions() {
    functions_ = {
            {"sin",   Lexem::sin},
            {"cos",   Lexem::cos},
            {"tan",   Lexem::tan},
            {"asin",  Lexem::a_sin},
            {"acos",  Lexem::a_cos},
            {"atan",  Lexem::a_tan},
            {"sqrt",  Lexem::sqrt},
            {"log",   Lexem::log},
            {"log10", Lexem::log10}};
}

void s21::CalcModel::initOperators() {
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
            {Lexem::sin,   0},
            {Lexem::cos,   0},
            {Lexem::tan,   0},
            {Lexem::a_sin, 0},
            {Lexem::a_cos, 0},
            {Lexem::a_tan, 0},
            {Lexem::sqrt,  0},
            {Lexem::log,   0},
            {Lexem::log10, 0},
            {Lexem::unary, 2},
            {Lexem::deg,   3},
            {Lexem::mul,   4},
            {Lexem::div,   4},
            {Lexem::mod,   4},
            {Lexem::sum,   5},
            {Lexem::sub,   5}};
}

void s21::CalcModel::validateExpression() {
    try {
        exprtk::symbol_table<double> symbol_table;
        exprtk::expression<double> expression;
        exprtk::parser<double> parser;
        symbol_table.add_constants();
        symbol_table.add_variable("X", x_value_);
        expression.register_symbol_table(symbol_table);
        substituteExpression();  // "log" -> "log10", "ln" -> "log", "mod" -> "%", "E" -> "*10^"
        parser.compile(expr_, expression);

//        Don't forget to reduce it
        result_ = expression.value();

    } catch (...) {
        expr_ = "Uncorrect expression";
    }
}

void s21::CalcModel::substituteExpression() {
    replaceInExpression("log", "log10");
    replaceInExpression("ln", "log");
    replaceInExpression("mod", "%");
    replaceInExpression("E", "*10^");
}

void s21::CalcModel::replaceInExpression(const std::string &from, const std::string &to) {
    size_t startPos = 0;
    while ((startPos = expr_.find(from, startPos)) != std::string::npos) {
        expr_.replace(startPos, from.length(), to);
        startPos += to.length();
    }
}

void s21::CalcModel::convertExpressionToPostfix() {
    std::stack<Token> operators;
    bool is_unary = true;
    int which = 0;
    size_t index = 0;
    size_t expression_length = expr_.length();
    while (index < expression_length) {
        if (isUnaryOperator(index, is_unary)) {
            handleUnaryOperator(index, operators, is_unary);
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

void s21::CalcModel::handleUnaryOperator(size_t &index, std::stack<Token> &operators, bool &is_unary) {
    if (expr_[index] == '-') {
        operators.emplace(LType::op, Lexem::unary);
    }
    ++index;
}

void s21::CalcModel::handleVariable(size_t &index, bool &is_unary) {
    postfix_vector_.emplace_back(LType::num, Lexem::num_x);
    is_unary = false;
    ++index;
}

void s21::CalcModel::handleFunction(int func_type, std::stack<Token> &operators, bool &is_unary) {
    operators.emplace(LType::func, static_cast<Lexem>(func_type));
    is_unary = false;
}

void s21::CalcModel::handleOpeningBrace(size_t &index, std::stack<Token> &operators, bool &is_unary) {
    operators.emplace(LType::op, Lexem::br_open);
    is_unary = true;
    ++index;
}

void s21::CalcModel::handleClosingBrace(size_t &index, std::stack<Token> &operators, bool &is_unary) {
    ++index;
    while (!operators.empty() && operators.top().getLexemName() != Lexem::br_open) {
        stackToVector(operators);
    }
    if (!operators.empty() && operators.top().getLexemName() == Lexem::br_open) {
        operators.pop();
    }
    is_unary = false;
}

void s21::CalcModel::handleNumeric(size_t &index, bool &is_unary) {
    postfix_vector_.emplace_back(LType::num, Lexem::num, extractDigit(index));
    is_unary = false;
}

void s21::CalcModel::handleOperator(size_t &index, std::stack<Token> &operators, bool &is_unary) {
    Lexem handlingOperator = charToLexem(expr_[index++]);
    while (!operators.empty() &&
           operators.top().getLexemName() != Lexem::br_open &&
           getPriority(operators.top().getLexemName()) <= getPriority(handlingOperator)) {
        if (handlingOperator == Lexem::deg && operators.top().getLexemName() == Lexem::deg) {
            break;
        }
        stackToVector(operators);
    }
    operators.emplace(LType::op, handlingOperator);
    is_unary = true;
}

void s21::CalcModel::stackToVector(std::stack<Token> &operators) {
    postfix_vector_.push_back(operators.top());
    operators.pop();
}

int s21::CalcModel::getPriority(const Lexem &lexem) {
    return priorities_.at(lexem);
}

double s21::CalcModel::extractDigit(size_t &pos) {
    size_t start = pos;
    while (pos < expr_.length() &&
           (std::isdigit(expr_[pos]) || expr_[pos] == '.'))
        ++pos;
    return std::stod(expr_.substr(start, pos - start));
}

s21::CalcModel::Lexem s21::CalcModel::charToLexem(const char &oper) {
    Lexem lexem;
    try {
        lexem = operators_.at(oper);
    } catch (const std::out_of_range &e) {
        expr_ = "Uncorrect expression";  // unknown lexem
        lexem = Lexem::br_open;
        return lexem;
    }
    return lexem;
}

