#ifndef A3_PARALLELS_SRC_GAUSS_MODEL_GAUSS_H_
#define A3_PARALLELS_SRC_GAUSS_MODEL_GAUSS_H_

#include <atomic>
#include <cmath>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "matrix.h"

class Barrier {
public:
  explicit Barrier(size_t n) : threadCount(n) {}

  Barrier(const Barrier &) = delete;
  ~Barrier() = default;

  void Wait();

private:
  const size_t threadCount;
  std::atomic<size_t> threadsWaiting{};
  std::condition_variable waitVariable;
  std::mutex mutex;
};

class Gauss {
public:
  Gauss(s21::Matrix<double> &matrix, int n_times)
      : n_times_(n_times), matrix_(matrix) {}
  s21::Matrix<double> GaussOneThreadNTimes();
  s21::Matrix<double> GaussMultiThreadNTimes();
  bool CheckValidData();

private:
  size_t n_times_ = 1;
  s21::Matrix<double> matrix_;
  s21::Matrix<double> GaussMethodOneThread();
  s21::Matrix<double> GaussMethodMultiThread();
  void ForwardMove(s21::Matrix<double> &, int, int, Barrier &);
  void BackMove(s21::Matrix<double> &, int, int, Barrier &);
};

#endif // A3_PARALLELS_SRC_GAUSS_MODEL_GAUSS_H_
