#include <clocale>

#include "view.h"

int main() {
  std::setlocale(LC_ALL, "C");
  View view;
  view.Start();
  return 0;
}
