#include <iostream>
#include <map>
#include <vector>
class myShell
{
  bool exit;
  std::string input;       //original input from user
  std::string validInput;  //valid input after removing unnecessary slash and space
  std::vector<std::string> arg;
  char ** argv;
  char * ECE551PATH = nullptr;  //environment variable
  std::vector<std::string> pathsVector;
  std::string cmdName;
  int arg_n;
  std::map<std::string, std::string> VarMap;
  bool cmdFound;  //equals 1 if command found

 public:
  myShell();   //constructor
  ~myShell();  //destructor
               //do we need a assignment operator?
  void runMyShell();
  void exitmyShell();
  bool getExit();
  void runCommand();
  int parseInput();
  void parsePATH();
  void searchCommand();
  void validateInput();
  void cdCommand();
  void prsSetCmd();
  void setVar();
  void exportVar();
  void incVar();
  void validateVarCmd();
  void destroyArgv();
};
