#ifndef A3_PARALLELS_SRC_ANTS_ANTS_ALG_H_
#define A3_PARALLELS_SRC_ANTS_ANTS_ALG_H_

#include <algorithm>
#include <future>
#include <mutex>
#include <random>
#include <unordered_set>

#include "graph.h"

namespace s21 {
class Pheromone {
private:
  Matrix<double> matrix_;
  const double evaporation_{0.2};

public:
  void Evaporation();
  explicit Pheromone(int edges) : matrix_(edges, edges) {}
  Pheromone() = default;
  ~Pheromone() = default;
  Pheromone &operator=(const Pheromone &m1);
  double const &operator()(int i, int j) const { return matrix_(i, j); }
  double &operator()(int i, int j) { return matrix_(i, j); }
  Matrix<double> const &get_matrix() const { return matrix_; }
  void clear() { matrix_.get_matrix().clear(); }
};

class Ant {
public:
  Ant() = default;
  std::pair<std::unordered_set<int>, double> Walk(Matrix<int> const Territory,
                                                  Pheromone &temp);
  ~Ant() = default;

private:
  double MakeDecision(Matrix<int> const &territory, Pheromone &matrixph_);
  int RandomLocation(int limit);
  void AddPheromone(Pheromone &pheromonmatrix, const Matrix<int> &Territory);
  double SumUnVisit(const Matrix<int> &territory, Pheromone &matrixph_);
  double length{0};
  std::unordered_set<int> Tabulist;
  int here_{};
};

class Colony {
public:
  explicit Colony(Graph &graph);
  ~Colony() = default;
  std::pair<std::unordered_set<int>, double> FindBestParth(int n);
  std::pair<std::unordered_set<int>, double> FindBestParthParralels(int n);

private:
  std::vector<Ant> ants_;
  Pheromone matrix_pheromones_;
  Matrix<int> territory_;
};

} // namespace s21
#endif // A3_PARALLELS_SRC_ANTS_ANTS_ALG_H_
