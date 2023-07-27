#include "common_view.h"

void CommonView::SetNumberOfExecutions() {
  ClearScreen();
  PrintMenu(menu_[2]);
  int item = ReadInput(1, 10000);
  count_of_executions_ = item;
}
void CommonView::ClearScreen() { system("clear"); }

void CommonView::RunMenu(
    std::vector<std::string> menu, int basic_start, int menu_min, int menu_max,
    std::map<unsigned, std::function<void()>> functions_map) {
  while (true) {
    PrintMenu(menu[basic_start]);
    int item = ReadInput(menu_min, menu_max);
    if (!item) break;
    functions_map[item]();
    ClearScreen();
  }
}

int CommonView::ReadInput(int min, int max) {
  std::string line;
  std::getline(std::cin, line);
  std::int64_t result;
  while (!sscanf(line.c_str(), "%lld", &result) || result < min ||
         result > max) {
    PrintMenu("| Incorrect input, try again >> ");
    std::getline(std::cin, line);
  }
  return result;
}

double CommonView::RandomNumber() {
  std::random_device rd;
  std::uniform_real_distribution<> dist(0, 5);
  return dist(rd);
}

void CommonView::FillRandom(s21::Matrix<double> &matrix) {
  for (int i = 0; i < matrix.Rows(); ++i) {
    for (int j = 0; j < matrix.Cols(); ++j) {
      matrix(i, j) = RandomNumber();
    }
  }
}

s21::Matrix<double> CommonView::TimeTracking(
    std::function<s21::Matrix<double>()> foo) {
  auto start = std::chrono::high_resolution_clock::now();
  s21::Matrix<double> result = foo();
  auto finish = std::chrono::high_resolution_clock::now();
  auto time =
      std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
  std::printf("Time taken: %.2f ms\n", time.count() / 1000.0);
  return result;
}

bool CommonView::CheckFile(std::ifstream &file) {
  bool res = false;
  std::string line;
  getline(file, line);
  if (std::regex_match(line, std::regex("^([0-9]*) [0-9]*$"))) {
    file.seekg(0, std::ios::beg);
    auto size = ParseSMatrixSize(file);
    if (size.first > 0 && size.second > 0) {
      if (CheckMatrix(file, size)) res = true;
      file.seekg(0, std::ios::beg);
    }
  }
  return res;
}

std::pair<int, int> CommonView::ParseSMatrixSize(std::ifstream &file) {
  std::string line;
  getline(file, line);
  size_t sub_pos = 0;
  int rows = std::stoi(line, &sub_pos);
  int cols = std::stoi(line.substr(sub_pos), &sub_pos);
  return std::make_pair(rows, cols);
}

bool CommonView::CheckMatrix(std::ifstream &file, std::pair<int, int> size) {
  std::string line;
  for (int j = 0; j < size.first; ++j) {
    std::getline(file, line);
    if (!std::regex_match(line, std::regex("^(\\d+ )*\\d+$"))) {
      return false;
    } else {
      int count = std::count(line.begin(), line.end(), ' ');
      if (count != size.second - 1) return false;
    }
  }
  return true;
}