#include "view.h"

View::View() : matrix_a_(300, 300), matrix_b_(300, 300) {
  ClearScreen();
  InitMenu();
  InitMatrixMenu();
  count_of_executions_ = 50;
  map_console_menu_ = {{1, [this]() { MatrixSettings(); }},
                       {2, [this]() { SetNumberOfExecutions(); }},
                       {3, [this]() { Calculate(); }}};
  map_matrix_menu_ = {{1, [this]() { LoadMatrixFromFile(matrix_a_); }},
                      {2, [this]() { LoadMatrixFromFile(matrix_b_); }},
                      {3, [this]() { SetMatrixRandomly(matrix_a_); }},
                      {4, [this]() { SetMatrixRandomly(matrix_b_); }},
                      {5, [this]() { ShowMatrixes(); }}};
  FillRandom(matrix_a_);
  FillRandom(matrix_b_);
}

View::~View() { ClearScreen(); }

void View::InitMenu() {
  menu_ = {{" -------------------------------------------------------- \n"
            "|                     WELCOME INTRO                      |\n"
            "|                        WINOGRAD                        |\n"
            "|          ALGORITHM OF MATRIX MULTIPLICATION            |\n"},
           {" -------------------------------------------------------- \n"
            "|                    SELECT MENU ITEM                    |\n"
            " -------------------------------------------------------- \n"
            "| 1 - Matrix settings                                    |\n"
            "| 2 - Set number of executions                           |\n"
            "| 3 - Start winograd calculate                           |\n"
            "| 0 - Exit                                               |\n"
            " -------------------------------------------------------- \n"
            "| >> "},
           {" ------------------------------------------------------- \n"
            "|       ENTER THE NUMBER OF EXECUTIONS (1 - 10000)      |\n"
            " ------------------------------------------------------- \n"
            "| >> "},
           {" -------------------------------------------------------- \n"
            "|              MATRIXES CANNOT BE MULTIPLIED             |\n"
            " -------------------------------------------------------- \n"
            "| 0 - Back                                               |\n"
            " -------------------------------------------------------- \n"
            "| >> "},
           {" -------------------------------------------------------- \n"
            "| 1 - Print one thread result matrix                     |\n"
            "| 2 - Print parallel result matrix                       |\n"
            "| 3 - Print pipeline result matrix                       |\n"
            "| 0 - Back                                               |\n"
            " -------------------------------------------------------- \n"
            "| >> "}};
}

void View::InitMatrixMenu() {
  matrix_menu_ = {
      {" -------------------------------------------------------- \n"
       "|                    MATRIX SETTINGS                     |\n"},
      {" -------------------------------------------------------- \n"
       "|                    SELECT MENU ITEM                    |\n"
       " -------------------------------------------------------- \n"
       "| 1 - Load matrix A from file                            |\n"
       "| 2 - Load matrix B from file                            |\n"
       "| 3 - Set matrix A randomly                              |\n"
       "| 4 - Set matrix B randomly                              |\n"
       "| 5 - Show matrixes                                      |\n"
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

void View::Start() {
  PrintMenu(menu_[0]);
  RunMenu(menu_, 1, 0, 3, map_console_menu_);
}

void View::PrintMenu(std::string string) {
  std::cout << termcolor::color<226> << termcolor::on_color<55> << string
            << termcolor::reset;
}

void View::PrintCalcResult(s21::Matrix<double> matrix) {
  std::cout << matrix << std::endl;
  PrintMenu("Press 0 (zero) to continue >> ");
  ReadInput(0, 0);
}

void View::MatrixSettings() {
  ClearScreen();
  PrintMenu(matrix_menu_[0]);
  RunMenu(matrix_menu_, 1, 0, 5, map_matrix_menu_);
}

void View::LoadMatrixFromFile(s21::Matrix<double> &matrix) {
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

void View::SetMatrixRandomly(s21::Matrix<double> &matrix) {
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

void View::ShowMatrixes() {
  ClearScreen();
  std::cout << "Matrix A:" << std::endl;
  std::cout << matrix_a_ << std::endl;
  std::cout << "Matrix B:" << std::endl;
  std::cout << matrix_b_ << std::endl;
  PrintMenu("Press 0 (zero) to continue >> ");
  ReadInput(0, 0);
}

void View::Calculate() {
  ClearScreen();
  Winograd alg(matrix_a_, matrix_b_, count_of_executions_);
  if (alg.CheckValidData()) {
    PrintMenu("Calculations start...\n\n");

    PrintMenu("One thread multiplication:\n");
    s21::Matrix<double> one_thread_result =
        TimeTracking([&]() { return alg.OneThreadMultiplication(); });

    PrintMenu("\nClassic parallels multiplication:\n");
    s21::Matrix<double> parallels_result;
    unsigned possible_threads = std::thread::hardware_concurrency();
    for (unsigned th_count = 2; th_count <= possible_threads * 4;
         th_count *= 2) {
      std::cout << th_count << " threads >> ";
      parallels_result =
          TimeTracking([&]() { return alg.ParallelMultiplication(th_count); });
    }

    PrintMenu("\nPipeline multiplication:\n");
    s21::Matrix<double> pipeline_result =
        TimeTracking([&]() { return alg.PipelineMultiplication(); });

    std::map<unsigned, std::function<void()>> calc_foo_map = {
        {1, [&]() { PrintCalcResult(one_thread_result); }},
        {2, [&]() { PrintCalcResult(parallels_result); }},
        {3, [&]() { PrintCalcResult(pipeline_result); }}};
    RunMenu(menu_, 4, 0, 4, calc_foo_map);
  } else {
    PrintMenu(menu_[3]);
    ReadInput(0, 0);
  }
}
