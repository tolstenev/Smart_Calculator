#include "../src/model/main_model.h"

#include <gtest/gtest.h>

#include <cmath>

using namespace s21;

class CalcTest : public testing::Test {
 protected:
  CalculatorModel calc_;
  std::string error_ = "Error";
  std::string err_div_zero_ = "1/0";
  std::string err_sqrt_uncorrect_ = "sqrt(-1)";
  std::string err_abracadabra_ = "1234g43s;;";
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
  std::string graph_func_ = "1/X";
  std::string graph_func_fail_ = "cocos(X)";
  std::vector<double> plot_limits_ = {-30, 30, -100, 100};
  std::pair<std::list<double>, std::list<double>> dots_;
  std::list<double> dots_x_, dots_y_;
};

TEST_F(CalcTest, DivisionByZeroFail) {
  calc_.Calculate(err_div_zero_);
  EXPECT_EQ(calc_.GetResultString(), error_);
}

TEST_F(CalcTest, SqrtUncorrectFail) {
  calc_.Calculate(err_sqrt_uncorrect_);
  EXPECT_EQ(calc_.GetResultString(), error_);
}

TEST_F(CalcTest, InvalidExpressionFail) {
  calc_.Calculate(err_abracadabra_);
  EXPECT_EQ(calc_.GetResultString(), error_);
}

TEST_F(CalcTest, SimpleLogSuccess) {
  calc_.Calculate(simple_log_);
  EXPECT_EQ(calc_.GetResultString(), simple_log_res_);
}

TEST_F(CalcTest, MultifoldSuccess) {
  calc_.Calculate(multifold_);
  EXPECT_EQ(calc_.GetResultString(), multifold_res_);
}

TEST_F(CalcTest, FunctionsSuccess) {
  calc_.Calculate(functions_t_);
  EXPECT_EQ(calc_.GetResultString(), functions_res_);
}

TEST_F(CalcTest, FoldedFunctionsSuccess) {
  calc_.Calculate(folded_funcs_);
  EXPECT_EQ(calc_.GetResultString(), folded_funcs_res_);
}

TEST_F(CalcTest, ExponentialNotationSuccess) {
  calc_.Calculate(exp_notation_);
  EXPECT_EQ(calc_.GetResultString(), exp_notation_res_);
}

TEST_F(CalcTest, ExponentialNotationHardSuccess) {
  calc_.Calculate(exp_not_simple_);
  EXPECT_EQ(calc_.GetResultString(), exp_not_simple_res_);
}

TEST_F(CalcTest, DegreeHardSuccess) {
  calc_.Calculate(degree_hard_);
  EXPECT_EQ(calc_.GetResultString(), degree_hard_res_);
}

TEST_F(CalcTest, TwoTimesCalculationSuccess) {
  calc_.Calculate(degree_hard_);
  EXPECT_EQ(calc_.GetResultString(), degree_hard_res_);
  calc_.Calculate(functions_t_);
  EXPECT_EQ(calc_.GetResultString(), functions_res_);
}

TEST_F(CalcTest, DegreeFunctionsSuccess) {
  calc_.Calculate(degree_funcs);
  EXPECT_EQ(calc_.GetResultString(), degree_funcs_res);
}

TEST_F(CalcTest, VariableXSuccess) {
  calc_.SetXValue(x_d_);
  calc_.Calculate(x_str_main_);
  EXPECT_EQ(calc_.GetResultString(), x_str_res_);
}

TEST_F(CalcTest, ModTestSuccess) {
  calc_.Calculate(mod_);
  EXPECT_EQ(calc_.GetResultString(), mod_res_);
}

TEST_F(CalcTest, PlotTestSuccess) {
  calc_.CalculateDots(graph_func_, plot_limits_);
  dots_ = calc_.GetDots();
  dots_x_ = dots_.first;
  dots_y_ = dots_.second;
  EXPECT_FALSE(dots_x_.empty());
  EXPECT_FALSE(dots_y_.empty());
}

TEST_F(CalcTest, PlotTestFail) {
  calc_.CalculateDots(graph_func_fail_, plot_limits_);
  EXPECT_EQ(calc_.GetResultString(), "Error");
  EXPECT_TRUE(dots_x_.empty());
  EXPECT_TRUE(dots_y_.empty());
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
