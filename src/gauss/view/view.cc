#include "view.h"

GaussView::GaussView() : matrix_(100, 101) {
  ClearScreen();
  InitMenu();
  InitMatrixMenu();
  count_of_executions_ = 50;
  map_console_menu_ = {{1, [this]() { MatrixSettings(); }},
                       {2, [this]() { SetNumberOfExecutions(); }},
                       {3, [this]() { Calculate(); }}};
  map_matrix_menu_ = {{1, [this]() { LoadMatrixFromFile(matrix_); }},
                      {2, [this]() { SetMatrixRandomly(matrix_); }},
                      {3, [this]() { ShowMatrixes(); }}};
  FillRandom(matrix_);
}

GaussView::~GaussView() { ClearScreen(); }

void GaussView::InitMenu() {
  menu_ = {{" -------------------------------------------------------- \n"
            "|                     WELCOME INTRO                      |\n"
            "|                         GAUSS                          |\n"
            "|               ALGORITHM OF SLAE SOLUTION               |\n"},
           {" -------------------------------------------------------- \n"
            "|                    SELECT MENU ITEM                    |\n"
            " -------------------------------------------------------- \n"
            "| 1 - Matrix settings                                    |\n"
            "| 2 - Set number of executions                           |\n"
            "| 3 - Start gauss calculate                              |\n"
            "| 0 - Exit                                               |\n"
            " -------------------------------------------------------- \n"
            "| >> "},
           {" ------------------------------------------------------- \n"
            "|       ENTER THE NUMBER OF EXECUTIONS (1 - 10000)      |\n"
            " ------------------------------------------------------- \n"
            "| >> "},
           {" -------------------------------------------------------- \n"
            "|                SLAE CANNOT BE RESOLVED                |\n"
            " -------------------------------------------------------- \n"
            "| 0 - Back                                               |\n"
            " -------------------------------------------------------- \n"
            "| >> "},
           {" -------------------------------------------------------- \n"
            "| 1 - Print one thread algorithm result                  |\n"
            "| 2 - Print parallel algorithm result                    |\n"
            "| 0 - Back                                               |\n"
            " -------------------------------------------------------- \n"
            "| >> "}};
}

void GaussView::InitMatrixMenu() {
  matrix_menu_ = {
      {" -------------------------------------------------------- \n"
       "|                    MATRIX SETTINGS                     |\n"},
      {" -------------------------------------------------------- \n"
       "|                    SELECT MENU ITEM                    |\n"
       " -------------------------------------------------------- \n"
       "| 1 - Load matrix from file                              |\n"
       "| 2 - Set matrix A randomly                              |\n"
       "| 3 - Show matrix                                        |\n"
       "| 0 - Back                                               |\n"
       " -------------------------------------------------------- \n"
       "| >> "},
      {" -------------------------------------------------------- \n"
       "|             INSERT PATH TO FILE FOR MATRIX             |\n"
       " -------------------------------------------------------- \n"
       "| >> "},
      {" -------------------------------------------------------- \n"
       "|                      WRONG FILE !                      |\n"
       " -------------------------------------------------------- \n"
       "| 1 - Try again                                          |\n"
       "| 0 - Back                                               |\n"
       " -------------------------------------------------------- \n"
       "| >> "},
      {" -------------------------------------------------------- \n"
       "|                    FILE NOT FOUND !                    |\n"
       " -------------------------------------------------------- \n"
       "| 1 - Try again                                          |\n"
       "| 0 - Back                                               |\n"
       " -------------------------------------------------------- \n"
       "| >> "},
      {" ------------------------------------------------------- \n"
       "|          ENTER THE NUMBER OF ROWS (1 - 10000)         |\n"
       " ------------------------------------------------------- \n"
       "| >> "},
      {" ------------------------------------------------------- \n"
       "|        ENTER THE NUMBER OF COLUMNS (1 - 10000)        |\n"
       " ------------------------------------------------------- \n"
       "| >> "}};
}

void GaussView::Start() {
  PrintMenu(menu_[0]);
  RunMenu(menu_, 1, 0, 3, map_console_menu_);
}

void GaussView::PrintMenu(std::string string) {
  std::cout << termcolor::color<231> << termcolor::on_color<52> << string
            << termcolor::reset;
}

void GaussView::MatrixSettings() {
  ClearScreen();
  PrintMenu(matrix_menu_[0]);
  RunMenu(matrix_menu_, 1, 0, 4, map_matrix_menu_);
}

void GaussView::Calculate() {
  ClearScreen();
  Gauss alg(matrix_, count_of_executions_);
  if (alg.CheckValidData()) {
    PrintMenu("Calculations start...\n\n");

    PrintMenu("One thread algorithm solution:\n");
    s21::Matrix<double> one_thread_result =
        TimeTracking([&]() { return alg.GaussOneThreadNTimes(); });

    PrintMenu("Parallel algorithm solution:\n");
    s21::Matrix<double> parallels_result;
    parallels_result =
        TimeTracking([&]() { return alg.GaussMultiThreadNTimes(); });

    std::map<unsigned, std::function<void()>> calc_foo_map = {
        {1, [&]() { PrintCalcResult(one_thread_result); }},
        {2, [&]() { PrintCalcResult(parallels_result); }}};
    RunMenu(menu_, 4, 0, 2, calc_foo_map);
  } else {
    PrintMenu(menu_[3]);
    ReadInput(0, 0);
  }
}

void GaussView::PrintCalcResult(s21::Matrix<double> matrix) {
  std::cout << matrix << std::endl;
  PrintMenu("Press 0 (zero) to continue >> ");
  ReadInput(0, 0);
}

void GaussView::LoadMatrixFromFile(s21::Matrix<double> &matrix) {
  std::string file_name;
  std::ifstream fload;
  while (true) {
    ClearScreen();
    PrintMenu(matrix_menu_[2]);
    std::cin >> file_name;
    std::cin.ignore();
    fload.open(file_name);
    if (fload) {
      if (!CheckFile(fload)) {
        ClearScreen();
        PrintMenu(matrix_menu_[3]);
        if (!ReadInput(0, 1))
          break;
      } else {
        auto size = ParseSMatrixSize(fload);
        s21::Matrix<double> tmp(size.first, size.second);
        fload >> tmp;
        matrix = tmp;
        break;
      }
    } else {
      ClearScreen();
      PrintMenu(matrix_menu_[4]);
      if (!ReadInput(0, 1))
        break;
    }
    fload.close();
  }
}

void GaussView::SetMatrixRandomly(s21::Matrix<double> &matrix) {
  ClearScreen();
  PrintMenu(matrix_menu_[5]);
  int rows = ReadInput(1, 10000);
  ClearScreen();
  PrintMenu(matrix_menu_[6]);
  int cols = ReadInput(1, 10000);
  s21::Matrix<double> tmp(rows, cols);
  FillRandom(tmp);
  matrix = tmp;
}

void GaussView::ShowMatrixes() {
  ClearScreen();
  std::cout << "Matrix:" << std::endl;
  std::cout << matrix_ << std::endl;
  PrintMenu("Press 0 (zero) to continue >> ");
  ReadInput(0, 0);
}