#include "winograd.h"

Winograd::Winograd(s21::Matrix<double> a, s21::Matrix<double> b, unsigned N)
    : matrix_a_(a), matrix_b_(b), count_executions_(N) {}

bool Winograd::CheckValidData() {
  if (matrix_a_.Cols() != matrix_b_.Rows()) {
    return false;
  }
  return true;
}

s21::Matrix<double> Winograd::MatrixMultiplication() {
  std::vector<double> row_factors = CalcRowFactors();
  std::vector<double> col_factors = CalcColFactors();
  s21::Matrix<double> result(matrix_a_.Rows(), matrix_b_.Cols());
  CalcMartix(result, row_factors, col_factors);
  CheckEvenMatrix(result);
  return result;
}

std::vector<double> Winograd::CalcRowFactors() {
  std::vector<double> row_factors(matrix_a_.Rows());
  for (int i = 0; i < matrix_a_.Rows(); ++i) {
    for (int j = 0; j < matrix_a_.Cols() / 2; ++j) {
      row_factors[i] += matrix_a_(i, j * 2) * matrix_a_(i, j * 2 + 1);
    }
  }
  return row_factors;
}

std::vector<double> Winograd::CalcColFactors() {
  std::vector<double> col_factors(matrix_b_.Cols());
  for (int i = 0; i < matrix_b_.Cols(); ++i) {
    for (int j = 0; j < matrix_b_.Rows() / 2; ++j) {
      col_factors[i] += matrix_b_(2 * j, i) * matrix_b_(2 * j + 1, i);
    }
  }
  return col_factors;
}

void Winograd::CalcMartix(s21::Matrix<double> &result,
                          std::vector<double> row_factors,
                          std::vector<double> col_factors) {
  for (int i = 0; i < result.Rows(); ++i) {
    for (int j = 0; j < result.Cols(); ++j) {
      double sum = 0;
      for (int k = 0; k < matrix_a_.Cols() / 2; ++k) {
        sum += (matrix_a_(i, k * 2) + matrix_b_(k * 2 + 1, j)) *
               (matrix_a_(i, k * 2 + 1) + matrix_b_(k * 2, j));
      }
      result(i, j) = sum - row_factors[i] - col_factors[j];
    }
  }
}

void Winograd::CheckEvenMatrix(s21::Matrix<double> &result) {
  if (matrix_a_.Cols() % 2) {
    for (int i = 0; i < matrix_a_.Rows(); ++i) {
      for (int j = 0; j < matrix_b_.Cols(); ++j) {
        result(i, j) += matrix_a_(i, matrix_a_.Cols() - 1) *
                        matrix_b_(matrix_a_.Cols() - 1, j);
      }
    }
  }
}

s21::Matrix<double> Winograd::OneThreadMultiplication() {
  s21::Matrix<double> matrix_result = MatrixMultiplication();
  for (unsigned i = 1; i < count_executions_; ++i) {
    MatrixMultiplication();
  }
  return matrix_result;
}

s21::Matrix<double> Winograd::ParallelMultiplication(unsigned th_count) {
  std::mutex result_mutex;
  s21::Matrix<double> result;
  std::vector<std::thread> threads_vector;
  int tasks_per_thread = count_executions_ / th_count;
  unsigned reminder = count_executions_ % th_count;
  bool change_flag = false;
  if (reminder) {
    tasks_per_thread += 1;
    change_flag = true;
  }
  for (unsigned i = 0; i < th_count; ++i) {
    if (i == reminder && change_flag)
      tasks_per_thread -= 1;
    threads_vector.push_back(std::thread([&]() {
      for (int j = 0; j < tasks_per_thread; ++j) {
        s21::Matrix<double> tmp = MatrixMultiplication();
        std::unique_lock<std::mutex> lock(mtx4_);
        result = tmp;
      }
    }));
  }
  for (auto &thread : threads_vector) {
    thread.join();
  }
  return result;
}

s21::Matrix<double> Winograd::PipelineMultiplication() {
  PipelineMemAlloc();
  std::thread th1([this]() { RowsFactorsPipeline(); });
  std::thread th2([this]() { ColsFactorsPipeline(); });
  std::thread th3([this]() { CalcMatrixesPipeline(); });
  std::thread th4([this]() { CheckEvenPipeline(); });
  th1.join();
  th2.join();
  th3.join();
  th4.join();
  return result_matrixes_pipeline_[0];
}

void Winograd::PipelineMemAlloc() {
  th1_flag_.resize(count_executions_);
  th2_flag_.resize(count_executions_);
  th3_flag_.resize(count_executions_);
  th4_flag_.resize(count_executions_);
  rows_factors_pipeline_.resize(count_executions_);
  cols_factors_pipeline_.resize(count_executions_);
  result_matrixes_pipeline_.resize(count_executions_);
  for (unsigned i = 0; i < count_executions_; ++i) {
    result_matrixes_pipeline_[i] =
        s21::Matrix<double>(matrix_a_.Rows(), matrix_b_.Cols());
  }
}

void Winograd::RowsFactorsPipeline() {
  for (unsigned i = 0; i < count_executions_; ++i) {
    rows_factors_pipeline_[i] = CalcRowFactors();
    {
      std::lock_guard<std::mutex> lock(mtx2_);
      th2_flag_[i] = true;
    }
    cv2_.notify_one();
  }
}

void Winograd::ColsFactorsPipeline() {
  for (unsigned i = 0; i < count_executions_; ++i) {
    std::unique_lock<std::mutex> lock(mtx2_);
    cv2_.wait(lock, [this, i] { return th2_flag_[i]; });
    cols_factors_pipeline_[i] = CalcColFactors();
    {
      std::lock_guard<std::mutex> lock(mtx3_);
      th3_flag_[i] = true;
    }
    cv3_.notify_one();
  }
}

void Winograd::CalcMatrixesPipeline() {
  for (unsigned i = 0; i < count_executions_; ++i) {
    std::unique_lock<std::mutex> lock(mtx3_);
    cv3_.wait(lock, [this, i] { return th3_flag_[i]; });
    CalcMartix(result_matrixes_pipeline_[i], rows_factors_pipeline_[i],
               cols_factors_pipeline_[i]);
    {
      std::lock_guard<std::mutex> lock(mtx4_);
      th4_flag_[i] = true;
    }
    cv4_.notify_one();
  }
}

void Winograd::CheckEvenPipeline() {
  for (unsigned i = 0; i < count_executions_; ++i) {
    std::unique_lock<std::mutex> lock(mtx4_);
    cv4_.wait(lock, [this, i] { return th4_flag_[i]; });
    CheckEvenMatrix(result_matrixes_pipeline_[i]);
  }
}
