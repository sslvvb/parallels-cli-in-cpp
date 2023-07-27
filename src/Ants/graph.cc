#include "graph.h"
namespace s21 {
void Graph::LoadGraphFromFile(const std::string &filename) {
  std::ifstream fload(filename);
  if (fload.is_open()) {
    if (IsValidFile(fload)) {
      int tops = TakeTopOfTheGraph(fload);
      adjacencymatrix_ = Matrix<int>(tops, tops);
      edges = tops;
      fload >> adjacencymatrix_;
      vert = std::make_unique<VertexOfTheGraph[]>(tops);
      edges = adjacencymatrix_.Rows();
      SaveEdges();
    } else {
      std::cout << "ERROR" << std::endl;
    }
  }
}

void Graph::ExportGraphToDot(const std::string &filename) {
  std::ofstream fout(filename);
  for (int i = 0; i < edges; i++) {
    fout << vert[i].number_of_edge << ';' << std::endl;
  }
  for (int i = 0; i < edges; i++) {
    for (auto t : vert[i].connects) {
      fout << vert[i].number_of_edge << " -- " << t << std::endl;
    }
  }
}

bool Graph::IsValidFile(std::ifstream &check) {
  bool res = false;
  std::string line;
  getline(check, line);
  if (CheckMatrixSize(line) == true) {
    check.seekg(0, std::ios::beg);
    int size = ParseSize(check);
    if (size > 0) {
      if (CheckMatrix(check, line, size) == true)
        res = true;
      check.seekg(0, std::ios::beg);
    }
  }
  return res;
}

int Graph::TakeTopOfTheGraph(std::ifstream &file) {
  std::string line;
  return getline(file, line), std::stoi(line, 0);
}

void Graph::SaveEdges() {
  int t = 1, n = 1;
  for (int i = 0; i < edges; i++) {
    for (int j = 0; j < edges; j++) {
      if (adjacencymatrix_(i, j) > 0) {
        vert[i].connects.push_back(t + j);
        vert[i].value_of_connect.push_back(adjacencymatrix_(i, j));
      }
    }
    vert[i].number_of_edge = n;
    n += 1;
  }
}

bool Graph::CheckMatrixSize(std::string line) {
  bool answer = true;
  if (std::regex_match(line, std::regex("^[0-9]*")) == false) {
    answer = false;
  }
  return answer;
}

int Graph::ParseSize(std::ifstream &file) {
  std::string line;
  getline(file, line);
  size_t sub_pos = 0;
  int number = std::stoi(line, &sub_pos);
  return number;
}

bool Graph::CheckMatrix(std::ifstream &file, std::string line, int size) {
  for (int j = 0; j < size; ++j) {
    std::getline(file, line);
    if (std::regex_match(line, std::regex("([0-9]* )*[0-9]*$")) == false) {
      return false;
    }
  }
  return true;
}
} // namespace s21
  // namespace s21
