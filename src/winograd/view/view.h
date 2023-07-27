#ifndef A3_PARALLELS_SRC_WINOGRAD_VIEW_VIEW_H_
#define A3_PARALLELS_SRC_WINOGRAD_VIEW_VIEW_H_

#include "common_view.h"
#include "termcolor.hpp"
#include "winograd.h"

class View : public CommonView {
public:
  View();
  ~View();
  void Start();

private:
  s21::Matrix<double> matrix_a_, matrix_b_;
  std::map<unsigned, std::function<void()>> map_console_menu_;
  std::map<unsigned, std::function<void()>> map_matrix_menu_;

  void InitMenu();
  void InitMatrixMenu();
  void PrintMenu(std::string string);
  void PrintCalcResult(s21::Matrix<double> matrix);
  void MatrixSettings();
  void Calculate();
  void LoadMatrixFromFile(s21::Matrix<double> &matrix);
  void SetMatrixRandomly(s21::Matrix<double> &matrix);
  void ShowMatrixes();
};

#endif // A3_PARALLELS_SRC_WINOGRAD_VIEW_VIEW_H_
