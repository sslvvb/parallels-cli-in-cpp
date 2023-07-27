#ifndef A3_PARALLELS_SRC_LIB_MATRIX_H_
#define A3_PARALLELS_SRC_LIB_MATRIX_H_

#include <fstream>
#include <iostream>
#include <vector>

namespace s21 {

template <typename T> class Matrix {
public:
  Matrix() = default;
  ~Matrix() = default;

  Matrix(std::initializer_list<std::initializer_list<T>> const &list) {
    size_t rows = list.size();
    size_t cols = rows ? list.begin()->size() : 0;
    if (rows == 0 && cols == 0)
      return;
    std::vector<T> vec;
    vec.reserve(rows * cols);
    for (auto &row : list) {
      if (row.size() != cols)
        return;
      vec.insert(vec.end(), row.begin(), row.end());
    }
    rows_ = rows, cols_ = cols;
    matrix_ = std::move(vec);
  }

  Matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0)
      return;
    rows_ = rows, cols_ = cols;
    matrix_ = std::vector<T>(rows * cols, 0);
  }

  Matrix(const Matrix &other)
      : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {}

  Matrix(Matrix &&other)
      : rows_(other.rows_), cols_(other.cols_),
        matrix_(std::move(other.matrix_)) {
    other.rows_ = other.cols_ = 0;
  }

  Matrix &operator=(const Matrix &other) {
    if (this != &other) {
      rows_ = other.rows_, cols_ = other.cols_;
      matrix_ = other.matrix_;
    }
    return *this;
  }

  Matrix &operator=(Matrix &&other) {
    if (this != &other) {
      rows_ = other.rows_, cols_ = other.cols_;
      matrix_ = std::move(other.matrix_);
      other.rows_ = other.cols_ = 0;
    }
    return *this;
  }

  bool operator==(const Matrix &other) const {
    return rows_ == other.rows_ && cols_ == other.cols_ &&
           matrix_ == other.matrix_;
  }

  friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix) {
    for (int i = 0; i < matrix.Rows(); ++i) {
      for (int j = 0; j < matrix.Cols(); ++j)
        out << matrix(i, j) << "\t";
      out << "\n";
    }
    return out;
  }

  friend std::ifstream &operator>>(std::ifstream &in, Matrix &matrix) {
    for (int i = 0; i < matrix.Size(); ++i) {
      in >> matrix(i / matrix.Cols(), i % matrix.Cols());
    }
    return in;
  }

  T const &operator()(int i, int j) const { return matrix_.at(j + i * cols_); }
  T &operator()(int i, int j) { return matrix_.at(j + i * cols_); }

  bool IsValid() const { return rows_ > 0 && cols_ > 0; }
  auto Begin() { return matrix_.begin(); }
  auto End() { return matrix_.end(); }
  int Rows() const { return rows_; }
  int Cols() const { return cols_; }
  int Size() const { return rows_ * cols_; }

  std::vector<T> &get_matrix() { return matrix_; }

private:
  int rows_{}, cols_{};
  std::vector<T> matrix_;
};

} // namespace s21

#endif // A3_PARALLELS_SRC_LIB_MATRIX_H_
