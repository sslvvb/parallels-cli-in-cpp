#include "gauss.h"

s21::Matrix<double> Gauss::GaussMethodMultiThread() {
  size_t row_number = matrix_.Rows();
  size_t thread_number = std::thread::hardware_concurrency();

  if (thread_number > row_number) {
    thread_number = row_number;
  }

  Barrier barrier(thread_number);
  Barrier phase(thread_number);
  std::vector<std::thread> threads;

  auto solve = [this, &barrier, &phase](int start, int end) {
    ForwardMove(matrix_, start, end, barrier);
    if (end == matrix_.Rows()) {
      matrix_(end - 1, end) /= matrix_(end - 1, end - 1);
    }
    phase.Wait();
    BackMove(matrix_, start, end, barrier);
  };

  int step = row_number / thread_number;
  for (size_t i = 0; i < thread_number; i++) {
    int start = i * step;
    int end = (i + 1) * step;
    threads.emplace_back(solve, start,
                         (i == thread_number - 1) ? matrix_.Rows() : end);
  }

  for (auto &thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  s21::Matrix<double> result(1, row_number);
  for (size_t i = 0; i < row_number; i++) {
    result(0, i) = matrix_(i, row_number);
  }
  return result;
}

void Gauss::ForwardMove(s21::Matrix<double> &matrix, int start, int end,
                        Barrier &barrier) {
  int row_number = matrix.Rows();
  int col_number = matrix.Cols();
  for (int k = 0; k < row_number; ++k) {
    for (int i = k + 1; i < row_number; ++i) {
      if (i >= start && i < end) {
        double K = matrix(i, k) / matrix(k, k);
        for (int j = 0; j < col_number; ++j) {
          matrix(i, j) -= K * matrix(k, j);
        }
      }
    }
    barrier.Wait();
  }
}

void Gauss::BackMove(s21::Matrix<double> &matrix, int start, int end,
                     Barrier &barrier) {
  int row_number = matrix.Rows();
  for (int k = row_number - 1; k > -1; k--) {
    for (int i = row_number; i > -1; i--) {
      matrix(k, i) = matrix(k, i) / matrix(k, k);
    }
    for (int i = k - 1; i > -1; i--) {
      if (i >= start && i < end) {
        double K = matrix(i, k) / matrix(k, k);
        for (int j = row_number; j > -1; j--) {
          matrix(i, j) = matrix(i, j) - matrix(k, j) * K;
        }
      }
    }
    barrier.Wait();
  }
}

s21::Matrix<double> Gauss::GaussMethodOneThread() {
  int row_number = matrix_.Rows();
  int col_number = matrix_.Cols();

  for (int k = 0; k < row_number; ++k) {
    for (int i = k + 1; i < row_number; ++i) {
      double K = matrix_(i, k) / matrix_(k, k);
      for (int j = 0; j < col_number; ++j) {
        matrix_(i, j) -= K * matrix_(k, j);
      }
    }
  }

  for (int k = row_number - 1; k > -1; k--) {
    for (int i = row_number; i > -1; i--) {
      matrix_(k, i) = matrix_(k, i) / matrix_(k, k);
    }
    for (int i = k - 1; i > -1; i--) {
      double K = matrix_(i, k) / matrix_(k, k);
      for (int j = row_number; j > -1; j--) {
        matrix_(i, j) = matrix_(i, j) - matrix_(k, j) * K;
      }
    }
  }

  s21::Matrix<double> result(1, row_number);
  for (int i = 0; i < row_number; i++) {
    result(0, i) = matrix_(i, row_number);
  }
  return result;
}

s21::Matrix<double> Gauss::GaussOneThreadNTimes() {
  s21::Matrix<double> result;
  for (size_t i = 0; i < n_times_; i++) {
    result = GaussMethodOneThread();
  }
  return result;
}

s21::Matrix<double> Gauss::GaussMultiThreadNTimes() {
  s21::Matrix<double> result;
  for (size_t i = 0; i < n_times_; i++) {
    result = GaussMethodMultiThread();
  }
  return result;
}

bool Gauss::CheckValidData() {
  if (matrix_.Cols() <= matrix_.Rows()) {
    return false;
  }
  return true;
}

void Barrier::Wait() {
  std::unique_lock<std::mutex> lock(mutex);
  if (threadsWaiting.fetch_add(1) >= threadCount - 1) {
    waitVariable.notify_all();
    threadsWaiting.store(0);
  } else {
    waitVariable.wait(lock);
  }
}