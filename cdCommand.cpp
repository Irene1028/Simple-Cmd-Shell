#include <string.h>
#include <unistd.h>

#include "myShell.h"
/*cdCommand*/
/*We use this function to realize cd command*/
void myShell::cdCommand() {
  if (arg_n > 2) {
    std::cerr << "cd needs at most one argument.\n";
    return;
  }
  if (argv[1] == NULL || strcmp(argv[1], "~") == 0) {
    char * home = getenv("HOME");
    if (chdir(home) != 0) {
      std::cerr << "Failed to go back to home path\n";
    }
  }
  else {
    if (chdir(argv[1]) != 0) {
      std::cerr << "Failed to change working dir\n";
    }
  }
}
