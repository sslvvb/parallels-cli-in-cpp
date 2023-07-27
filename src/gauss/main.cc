#include <clocale>

#include "view.h"

int main() {
  std::setlocale(LC_ALL, "C");
  GaussView view;
  view.Start();
  return 0;
}