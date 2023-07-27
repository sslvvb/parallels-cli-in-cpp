#ifndef A3_PARALLELS_SRC_ANTS_GRAPH_H_
#define A3_PARALLELS_SRC_ANTS_GRAPH_H_

#include <regex>

#include "matrix.h"

namespace s21 {

struct VertexOfTheGraph {
  int number_of_edge;
  std::vector<int> connects;
  std::vector<int> value_of_connect;
};

class Graph {
public:
  Graph() {}
  ~Graph() {}

  void LoadGraphFromFile(const std::string &filename);
  void ExportGraphToDot(const std::string &filename);
  bool IsValidFile(std::ifstream &check);

  Matrix<int> &get_matrix() { return adjacencymatrix_; }
  int get_edges() { return edges; }
  std::shared_ptr<VertexOfTheGraph[]> &get_edges_matrix() { return vert; }

private:
  int edges{};
  Matrix<int> adjacencymatrix_;
  std::shared_ptr<VertexOfTheGraph[]> vert;

  int TakeTopOfTheGraph(std::ifstream &file);
  void SaveEdges();
  bool CheckMatrixSize(std::string line);
  int ParseSize(std::ifstream &file);
  bool CheckMatrix(std::ifstream &file, std::string line, int size);
};

} // namespace s21

#endif // A3_PARALLELS_SRC_ANTS_GRAPH_H_
