#ifndef A3_PARALLELS_SRC_ANTS_VIEW_H_
#define A3_PARALLELS_SRC_ANTS_VIEW_H_

#include "common_view.h"
#include "gauss.h"
#include "termcolor.hpp"

class GaussView : public CommonView {
public:
  GaussView();
  ~GaussView();
  void Start();

private:
  s21::Matrix<double> matrix_;
  std::map<unsigned, std::function<void()>> map_console_menu_;
  std::map<unsigned, std::function<void()>> map_matrix_menu_;

  void InitMenu();
  void Calculate();
  void ShowMatrixes();
  void MatrixSettings();
  void InitMatrixMenu();
  void PrintMenu(std::string string);
  void PrintCalcResult(s21::Matrix<double> matrix);
  void LoadMatrixFromFile(s21::Matrix<double> &matrix);
  void SetMatrixRandomly(s21::Matrix<double> &matrix);
};

#endif // A3_PARALLELS_ANTS_VIEW_H_
