#include "ants_alg.h"
template <typename T> void ConcurrentPrint(T const &value) {
  static std::mutex mtx;

  std::scoped_lock<std::mutex> lock(mtx);

  std::cout << value << std::endl;
}

namespace s21 {
std::pair<std::unordered_set<int>, double>
Ant::Walk(Matrix<int> const Territory, Pheromone &pheromonmatrix) {
  int t = RandomLocation(Territory.Rows());
  here_ = t == 0 ? t : t - 1;
  Tabulist.insert(here_);
  int cont = Territory.Cols() - 1;
  while (cont > 0) {
    here_ = MakeDecision(Territory, pheromonmatrix);
    cont--;
  }
  AddPheromone(pheromonmatrix, Territory);
  auto result = std::make_pair(Tabulist, length);
  length = 0;
  Tabulist.clear();
  here_ = 0;
  return result;
}

void Ant::AddPheromone(Pheromone &pheromonmatrix,
                       const Matrix<int> &Territory) {
  for (auto i = Tabulist.begin(); i != Tabulist.end(); ++i) {
    if (std::next(i, 1) != Tabulist.end()) {
      length += Territory(*i, *std::next(i, 1));
      pheromonmatrix(*i, *std::next(i, 1)) =
          1. / Territory(*i, *std::next(i, 1));
    }
  }
}

double Ant::SumUnVisit(const Matrix<int> &territory, Pheromone &matrixph_) {
  double result = 0;
  for (int j = 0; j < territory.Cols(); j++)
    if (territory(here_, j) > 0 && Tabulist.count(j) == 0)
      result += territory(here_, j) + matrixph_(here_, j);
  return result;
}

double Ant::MakeDecision(Matrix<int> const &territory, Pheromone &matrixph_) {
  std::vector<std::pair<int, double>> result;
  double sum = SumUnVisit(territory, matrixph_);

  for (int j = 0; j < territory.Cols(); j++) {
    if (territory(here_, j) > 0 && Tabulist.count(j) == 0) {
      auto smth =
          std::make_pair(j, (territory(here_, j) + matrixph_(here_, j)) / sum);
      result.push_back(smth);
    }
  }
  if (result.size() == 0)
    return here_;
  auto p = min_element(
      result.begin(), result.end(),
      [](const auto &lhs, const auto &rhs) { return lhs.second < rhs.second; });

  Tabulist.insert(p->first);

  return p->first;
}

int Ant::RandomLocation(int limit) {
  std::random_device rd;
  std::default_random_engine engine(rd());
  std::uniform_int_distribution<int> dist(0, limit);
  return (dist(engine));
}

Pheromone &Pheromone::operator=(const Pheromone &m1) {
  matrix_ = m1.matrix_;
  return *this;
}

std::pair<std::unordered_set<int>, double>
Colony::FindBestParthParralels(int n) {
  matrix_pheromones_ = Pheromone(territory_.Rows());
  std::vector<std::pair<std::unordered_set<int>, double>> best_paths;
  std::mutex mtx;
  for (int i = 0; i < n; i++) {
    std::vector<std::future<void>> futures;
    for (auto &temp : ants_) {
      futures.emplace_back(std::async(std::launch::async, [&] {
        auto t = temp.Walk(territory_, matrix_pheromones_);
        matrix_pheromones_.Evaporation();
        std::lock_guard<std::mutex> lock(mtx);
        best_paths.emplace_back(t);
      }));
    }
    for (auto &future : futures) {
      future.wait();
    }
  }
  auto min_path =
      std::min_element(best_paths.begin(), best_paths.end(),
                       [](const std::pair<std::unordered_set<int>, double> &a,
                          const std::pair<std::unordered_set<int>, double> &b) {
                         return a.second < b.second;
                       });
  matrix_pheromones_.clear();
  return *min_path;
}

std::pair<std::unordered_set<int>, double> Colony::FindBestParth(int n) {
  matrix_pheromones_ = Pheromone(territory_.Rows());
  std::vector<std::pair<std::unordered_set<int>, double>> best_paths;
  for (int i = 0; i < n; i++) {
    for (auto &temp : ants_) {
      auto t = temp.Walk(territory_, matrix_pheromones_);
      best_paths.emplace_back(t);
      matrix_pheromones_.Evaporation();
    }
  }
  auto min_path =
      std::min_element(best_paths.begin(), best_paths.end(),
                       [](const std::pair<std::unordered_set<int>, double> &a,
                          const std::pair<std::unordered_set<int>, double> &b) {
                         return a.second < b.second;
                       });
  matrix_pheromones_.clear();
  return *min_path;
}

Colony::Colony(s21::Graph &graph)
    : ants_(graph.get_edges()), territory_(graph.get_matrix()) {}

void Pheromone::Evaporation() {
  std::for_each(matrix_.Begin(), matrix_.End(),
                [&](auto &t) { t *= evaporation_; });
}
} // namespace s21