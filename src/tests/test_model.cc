#include <gtest/gtest.h>

#include <cmath>

#include "../src/model/main_model.h"

using namespace s21;

class CalcTest : public testing::Test {
 protected:
  CalcModel calc_;
  std::string error_ = "Error";
  std::string err_div_zero_ = "1/0";
  std::string err_sqrt_oppos_ = "sqrt(-1)";
  std::string err_abracadabra_ = "1234g43s;;";
  std::string err_empty_ = "";
  std::string fail_expr_ = "1x";
  std::string fail_lexem_ = "1;02941257";
  std::string fail_x_lexem_ = "qwerty";
  std::string fail_x_lexem_expr_ = "sin(x)";
  std::string simple_log_ = "log(4)";
  std::string simple_log_res_ = "0.60205999";
  std::string multifold_ =
      "15/(7-(1+1))*3-(2+(1+1))*15/(7-(200+1))*3-(2+(1+1))*(15/"
      "(7-(1+1))*3-(2+(1+1))+15/(7-(1+1))*3-(2+(1+1)))";
  std::string multifold_res_ = "-30.07216495";
  std::string functions_t_ =
      "sin(1)+cos(1)+asin(1)+acos(1)+atan(1)+tan(1)+sqrt(16)+ln(10)+log(10)";
  std::string functions_res_ = "12.5979606";
  std::string folded_funcs_ =
      "(132+sin(asin(sqrt(ln(log(228.11)))))-4*5^6*(123))";
  std::string folded_funcs_res_ = "-7687367.07378458";
  std::string exp_notation_ = "2.5*10^3+1.8*10^-2*(3.7E-5+2.1E2)";
  std::string exp_notation_res_ = "2503.78000067";
  std::string exp_not_simple_ = "1.5E5";
  std::string exp_not_simple_res_ = "150000";
  std::string degree_hard_ = "2^3^4";
  std::string degree_hard_res_ = "2417851639229258349412352";
  std::string degree_funcs = "sin(.2)^(cos(1)+tan(1.1))^sin(.6)";
  std::string degree_funcs_res = "0.06624972";
  std::string x_str_main_ = "sqrt((7.2+3.5-2.8)/(5.6*4.2))+sin(X)-cos(1.3)";
  double x_d_ = 0.8;
  std::string x_str_res_ = "1.02941257";
  std::string mod_ =
      "((sin(2.3)*(sqrt(7.8)+cos(1.2)))mod4.5)/(log(5.6)+atan(0.9))";
  std::string mod_res_ = "1.588689";
  //    std::string graph_func_ = "x^2";
  //    double x_begin_ = -30;
  //    double y_begin_ = -30;
  //    double x_end_ = 30;
  //    double y_end_ = 900;
  //    std::pair<std::vector<double>, std::vector<double>> vector_;
  //    std::vector<double> XVector, YVector;
};

TEST_F(CalcTest, DivByZero) {
  EXPECT_EQ(calc_.getResult(err_div_zero_), error_);
}

TEST_F(CalcTest, sqrtOpp) {
  EXPECT_EQ(calc_.getResult(err_sqrt_oppos_), error_);
}

TEST_F(CalcTest, invExpr) {
  EXPECT_EQ(calc_.getResult(err_abracadabra_), error_);
}

TEST_F(CalcTest, SimpleLog) {
  EXPECT_EQ(calc_.getResult(simple_log_), simple_log_res_);
}

TEST_F(CalcTest, Multifold) {
  EXPECT_EQ(calc_.getResult(multifold_), multifold_res_);
}

TEST_F(CalcTest, Functions) {
  EXPECT_EQ(calc_.getResult(functions_t_), functions_res_);
}

TEST_F(CalcTest, Folded_funcs) {
  EXPECT_EQ(calc_.getResult(folded_funcs_), folded_funcs_res_);
}

TEST_F(CalcTest, Exp_notation) {
  EXPECT_EQ(calc_.getResult(exp_notation_), exp_notation_res_);
}

TEST_F(CalcTest, expNSimple) {
  EXPECT_EQ(calc_.getResult(exp_not_simple_), exp_not_simple_res_);
}

TEST_F(CalcTest, degreeHard) {
  EXPECT_EQ(calc_.getResult(degree_hard_), degree_hard_res_);
}

TEST_F(CalcTest, degreeFuncs) {
  EXPECT_EQ(calc_.getResult(degree_funcs), degree_funcs_res);
}

 TEST_F(CalcTest, xStrEnableDouble) {
     calc_.setXValue(x_d_);
     EXPECT_EQ(calc_.getResult(x_str_main_), x_str_res_);
 }

TEST_F(CalcTest, modTest) { EXPECT_EQ(calc_.getResult(mod_), mod_res_); }

// TEST_F(CalcTest, graphTest) {
//     calc_.setExpr(graph_func_);
//     calc_.validateExpr();
//     calc_.convertExpr();
//     vector_ = calc_.getGraphVector(x_begin_, x_end_, y_begin_, y_end_);
//     XVector = vector_.first;
//     YVector = vector_.second;
//     EXPECT_FALSE(XVector.empty());
//     EXPECT_FALSE(XVector.empty());
// }

// TEST_F(CalcTest, graphTestFail) {
//     vector_ = calc_.getGraphVector(x_begin_, x_end_, y_begin_, y_end_);
//     XVector = vector_.first;
//     YVector = vector_.second;
//     EXPECT_TRUE(XVector.empty());
//     EXPECT_TRUE(XVector.empty());
// }

// TEST_F(CalcTest, graphAxisFail) {
//     calc_.setExpr(graph_func_);
//     calc_.validateExpr();
//     calc_.convertExpr();
//     vector_ = calc_.getGraphVector(x_end_, x_begin_, y_begin_, y_end_);
//     XVector = vector_.first;
//     YVector = vector_.second;
//     EXPECT_TRUE(XVector.empty());
//     EXPECT_TRUE(XVector.empty());
// }

// TEST_F(CalcTest, lexemFail) {
//     calc_.setExpr(fail_expr_);
//     calc_.setXValue(x_str_);
//     calc_.validateExpr();
//     calc_.convertExpr();
//     calc_.calculateExpr();
//     EXPECT_EQ(calc_.getStatus().first, 50);
//     EXPECT_EQ(calc_.getResult(), error_);
// }

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}