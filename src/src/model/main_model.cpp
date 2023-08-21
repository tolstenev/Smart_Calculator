#include "main_model.h"

s21::CalcModel::CalcModel() {
    mapsObjectsInit();
}

void s21::CalcModel::mapsObjectsInit() {
    functions_ = {
            {"sin", Lexem::sin},   {"cos", Lexem::cos},   {"tan", Lexem::tan},
            {"asin", Lexem::a_sin}, {"acos", Lexem::a_cos}, {"atan", Lexem::a_tan},
            {"sqrt", Lexem::sqrt}, {"log", Lexem::log},   {"log10", Lexem::log10}};
    operators_ = {{'^', Lexem::deg}, {'*', Lexem::mul},  {'/', Lexem::div},
                  {'%', Lexem::mod}, {'+', Lexem::sum}, {'-', Lexem::sub}};
    priorities_ = {{Lexem::sin, 0},   {Lexem::cos, 0},  {Lexem::tan, 0},
                   {Lexem::a_sin, 0},  {Lexem::a_cos, 0}, {Lexem::a_tan, 0},
                   {Lexem::sqrt, 0},  {Lexem::log, 0},  {Lexem::log10, 0},
                   {Lexem::unary, 2}, {Lexem::deg, 3},  {Lexem::mul, 4},
                   {Lexem::div, 4},   {Lexem::mod, 4},  {Lexem::sum, 5},
                   {Lexem::sub, 5}};
}

void s21::CalcModel::setExpression(const std::string &expr) {
    expr_ = expr;
}

