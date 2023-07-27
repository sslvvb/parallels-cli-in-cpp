#include "ants_alg.h"
#include "common_view.h"
#include "termcolor.hpp"
class AntView : public CommonView {
public:
  AntView();
  ~AntView() = default;
  void Start();

private:
  s21::Graph graph_;
  void InputGraph();
  void StartAnts();
  std::map<unsigned, std::function<void()>> map_console_menu_;
  void PrintMenu(std::string string);
  void InitMenu();
};
