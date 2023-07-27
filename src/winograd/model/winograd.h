#ifndef A3_PARALLELS_SRC_WINOGRAD_MODEL_WINOGRAD_H_
#define A3_PARALLELS_SRC_WINOGRAD_MODEL_WINOGRAD_H_

#include <mutex>
#include <thread>

#include "matrix.h"

class Winograd {
public:
  Winograd() = delete;
  Winograd(s21::Matrix<double> matrix_a, s21::Matrix<double> matrix_b,
           unsigned N);
  Winograd(const Winograd &other) = delete;
  Winograd(Winograd &&other) = delete;
  ~Winograd() = default;

  bool CheckValidData();
  s21::Matrix<double> OneThreadMultiplication();
  s21::Matrix<double> ParallelMultiplication(unsigned th_count);
  s21::Matrix<double> PipelineMultiplication();

private:
  const s21::Matrix<double> matrix_a_;
  const s21::Matrix<double> matrix_b_;
  const unsigned count_executions_;
  std::mutex mtx1_, mtx2_, mtx3_, mtx4_;
  std::condition_variable cv1_, cv2_, cv3_, cv4_;
  std::vector<bool> th1_flag_, th2_flag_, th3_flag_, th4_flag_;
  std::vector<std::vector<double>> rows_factors_pipeline_;
  std::vector<std::vector<double>> cols_factors_pipeline_;
  std::vector<s21::Matrix<double>> result_matrixes_pipeline_;

  s21::Matrix<double> MatrixMultiplication();
  std::vector<double> CalcRowFactors();
  std::vector<double> CalcColFactors();
  void CalcMartix(s21::Matrix<double> &result, std::vector<double> row_factors,
                  std::vector<double> col_factors);
  void CheckEvenMatrix(s21::Matrix<double> &result);
  void PipelineMemAlloc();
  void RowsFactorsPipeline();
  void ColsFactorsPipeline();
  void CalcMatrixesPipeline();
  void CheckEvenPipeline();
};

#endif // A3_PARALLELS_SRC_WINOGRAD_MODEL_WINOGRAD_H_
