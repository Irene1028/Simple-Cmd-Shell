#include <cstdlib>

#include "myShell.h"

int main() {
  myShell myShell;
  while (!myShell.getExit()) {
    myShell.runMyShell();
  }
  return EXIT_SUCCESS;
}
