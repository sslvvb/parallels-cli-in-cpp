#include "view.h"
AntView::AntView() {
  ClearScreen();
  InitMenu();
  map_console_menu_.emplace(1, [this]() { InputGraph(); });
  map_console_menu_.emplace(2, [this]() { SetNumberOfExecutions(); });
  map_console_menu_.emplace(3, [this]() { StartAnts(); });
}
void AntView::Start() {
  PrintMenu(menu_[0]);
  RunMenu(menu_, 1, 0, 3, map_console_menu_);
}
void AntView::InitMenu() {
  menu_ = {{" -------------------------------------------------------- \n"
            "|                     WELCOME INTRO                      |\n"
            "|                     Ants Algorithm Parallels           |\n"},
           {" -------------------------------------------------------- \n"
            "|                    SELECT MENU ITEM                    |\n"
            " -------------------------------------------------------- \n"
            "| 1 - Load Matrix                                        |\n"
            "| 2 - Set number of executions                           |\n"
            "| 3 - Start Ant Algorithm                                |\n"
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

void AntView::PrintMenu(std::string string) {
  std::cout << termcolor::color<231> << termcolor::on_color<33> << string
            << termcolor::reset;
}

void AntView::InputGraph() {
  PrintMenu("Input File Path");
  std::string file_name;
  std::cin >> file_name;
  std::cin.ignore();
  graph_.LoadGraphFromFile(file_name);
}

void AntView::StartAnts() {
  if (graph_.get_edges() > 0) {
    s21::Colony colony(graph_);
    auto start_time = std::chrono::high_resolution_clock::now();
    auto best_path = colony.FindBestParth(count_of_executions_);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                        end_time - start_time)
                        .count();
    std::cout << "Time taken by FindBestPath: " << duration << " milliseconds"
              << std::endl;
    std::cout << "path " << best_path.second << std::endl;
    auto start_timeg = std::chrono::high_resolution_clock::now();
    auto best_pathg = colony.FindBestParthParralels(count_of_executions_);
    auto end_timeg = std::chrono::high_resolution_clock::now();
    auto durationg = std::chrono::duration_cast<std::chrono::milliseconds>(
                         end_timeg - start_timeg)
                         .count();
    std::cout << "Time taken by FindBestPathParallel: " << durationg
              << " milliseconds" << std::endl;
    std::cout << "path " << best_pathg.second << std::endl;
    PrintMenu("Press 0 (zero) to continue >> ");
    ReadInput(0, 0);
  } else {
    std::cout << "ERROR " << std::endl;
  }
}