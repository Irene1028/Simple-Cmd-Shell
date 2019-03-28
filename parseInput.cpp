#include <string.h>

#include <vector>

#include "myShell.h"

/*parseInput()*/
//parse input and transform it to a good array: argv[]
int myShell::parseInput() {
  //if input is valid, we process it here
  //std::cout << "My command is:" << validInput << std::endl;
  std::string single_arg;
  for (std::string::iterator it = validInput.begin(); it != validInput.end(); it++) {
    std::string::iterator it2 = it;
    it2++;  //very important, can not write it2=++it, it will break loop!
    if (isspace(*it) == 0) {
      //if current is not space, push it in single_arg
      if (*it == '\0') {
        single_arg.push_back(' ');
      }
      else {
        single_arg.push_back(*it);
      }
      if (it2 == validInput.end()) {
        //if next is end of input, push single_arg to arg
        arg.push_back(single_arg);
        single_arg.clear();
      }
    }
    if (isspace(*it) != 0) {
      //if current is space
      if (it != validInput.begin()) {
        //and current is not the start of input
        std::string::iterator it1 = it;
        it1--;
        if (isspace(*it1) == 0) {
          // and previous one is not space
          // push single_arg to arg
          arg.push_back(single_arg);
          single_arg.clear();
        }
      }
    }
  }
  //process var names with '$'
  validateVarCmd();

  /*transform vector to array*/
  int j = 0;
  argv = new char *[arg.size() + 1];
  for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); ++it) {
    argv[j] = new char[it->length() + 1];
    strcpy(argv[j], (*it).c_str());
    j++;
  }
  argv[j] = nullptr;  //the final element of argv should be NULL

  return j;
}
