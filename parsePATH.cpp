#include <string.h>

#include <vector>

#include "myShell.h"

/*parsePATH()*/
void myShell::parsePATH() {
  ECE551PATH = getenv("PATH");
  //We initialize ECE551PATH at ini_list in default constructor
  char * pth = nullptr;
  char * copyPATH = strdup(ECE551PATH);  //avoid modify original PATH
  char * path_p = copyPATH;
  pth = strsep(&path_p, ":");
  while (pth != nullptr) {
    // for each path, push to pathsvector
    std::string path = pth;
    pathsVector.push_back(path);
    pth = strsep(&path_p, ":");
  }
  free(copyPATH);
}
