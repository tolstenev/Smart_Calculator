#include "main_window.h"

#include "./ui_main_window.h"

s21::CalculatorWindow::CalculatorWindow(CalculatorController &controller,
                                        QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), controller_(controller) {
  ui_->setupUi(this);
  setWindowTitle("SmartCalc by Yonn Argelia @yonnarge");
  setFixedSize(415, 725);
  ConnectSlots();
  InitPlot();
}

void s21::CalculatorWindow::InitPlot() {
  QCPGraph *graph = ui_->widget_plot->addGraph();
  QColor transparent_color(0, 0, 0, 0);
  ui_->widget_plot->setBackground(QBrush(transparent_color));

  ui_->widget_plot->xAxis->setBasePen(QPen(Qt::gray));
  ui_->widget_plot->yAxis->setBasePen(QPen(Qt::gray));
  ui_->widget_plot->xAxis->setTickPen(QPen(Qt::gray));
  ui_->widget_plot->yAxis->setTickPen(QPen(Qt::gray));
  ui_->widget_plot->xAxis->setTickLabelColor(Qt::gray);
  ui_->widget_plot->yAxis->setTickLabelColor(Qt::gray);
}

s21::CalculatorWindow::~CalculatorWindow() { delete ui_; }

void s21::CalculatorWindow::keyPressEvent(QKeyEvent *event) {
  if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_W) {
    close();  // Закрытие окна по сочетанию клавиш Cmd+W
  } else {
    QMainWindow::keyPressEvent(event);
  }
}

void s21::CalculatorWindow::PrintSymbols() {
  auto *button = qobject_cast<QPushButton *>(sender());
  QString new_result = ui_->line_expr->text() + button->text();
  ui_->line_expr->setText(new_result);
}

void s21::CalculatorWindow::ConnectSlots() {
  QShortcut *sc_equal = new QShortcut(QKeySequence(Qt::Key_Equal), this);
  std::list<QPushButton *> entering_buttons = {
      ui_->button_0,
      ui_->button_1,
      ui_->button_2,
      ui_->button_3,
      ui_->button_4,
      ui_->button_5,
      ui_->button_6,
      ui_->button_7,
      ui_->button_8,
      ui_->button_9,
      ui_->button_open_bracket,
      ui_->button_close_bracket,
      ui_->button_sum,
      ui_->button_sub,
      ui_->button_mul,
      ui_->button_div,
      ui_->button_dot,
      ui_->button_degree,
      ui_->button_x,
      ui_->button_e,
      ui_->button_sin,
      ui_->button_cos,
      ui_->button_tan,
      ui_->button_asin,
      ui_->button_acos,
      ui_->button_atan,
      ui_->button_log,
      ui_->button_ln,
      ui_->button_mod,
      ui_->button_sqrt,
  };

  for (auto button : entering_buttons) {
    connect(button, SIGNAL(clicked()), this, SLOT(PrintSymbols()));
  }

  connect(ui_->button_ac, SIGNAL(clicked()), this, SLOT(ClearAll()));
  connect(ui_->button_bs, SIGNAL(clicked()), this, SLOT(DeleteLastSymbol()));
  connect(ui_->button_plot, SIGNAL(clicked()), this, SLOT(CreatePlot()));
  connect(ui_->button_calculate, SIGNAL(clicked()), this, SLOT(Calculate()));
  connect(sc_equal, SIGNAL(activated()), this, SLOT(Calculate()));
}

void s21::CalculatorWindow::ClearAll() {
  ui_->line_expr->setText("");
  ui_->line_res->setText("");
  ui_->widget_plot->clearGraphs();
  ui_->widget_plot->replot();
}

void s21::CalculatorWindow::DeleteLastSymbol() {
  QString new_result = ui_->line_expr->text();
  new_result = new_result.left(new_result.length() - 1);
  ui_->line_expr->setText(new_result);
}

void s21::CalculatorWindow::Calculate() {
  controller_.SetXValue(ui_->double_spin_box_x->value());
  std::string expression = ui_->line_expr->text().toStdString();
  std::string result = controller_.Calculate(expression);
  ui_->line_res->setText(QString::fromStdString(result));
}

void s21::CalculatorWindow::CreatePlot() {
  double x_min = ui_->spin_box_min_horizontal->value();
  double x_max = ui_->spin_box_max_horizontal->value();
  double y_min = ui_->spin_box_min_vertical->value();
  double y_max = ui_->spin_box_max_vertical->value();
  std::vector<double> plot_limits = {x_min, x_max, y_min, y_max};
  controller_.SetXValue(ui_->double_spin_box_x->value());
  std::string expression = ui_->line_expr->text().toStdString();
  std::string result = controller_.Calculate(expression);

  ui_->line_res->setText(QString::fromStdString(result));

  std::pair<std::list<double>, std::list<double>> dots =
      controller_.CalculateDots(expression, plot_limits);

  ui_->widget_plot->clearGraphs();

  ui_->widget_plot->xAxis->setRange(x_min, x_max);
  ui_->widget_plot->yAxis->setRange(y_min, y_max);
  ui_->widget_plot->addGraph();
  QList<double> dots_x(dots.first.begin(), dots.first.end());
  QList<double> dots_y(dots.second.begin(), dots.second.end());
  ui_->widget_plot->graph(0)->addData(dots_x, dots_y, true);
  FormatPlotLine();
  ui_->widget_plot->replot();
}

void s21::CalculatorWindow::FormatPlotLine() {
  QPen pen(Qt::red);
  ui_->widget_plot->graph(0)->setPen(pen);
  ui_->widget_plot->graph(0)->setLineStyle(QCPGraph::lsNone);
  ui_->widget_plot->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssDisc, 1.5));
}