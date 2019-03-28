#include <dirent.h>
#include <string.h>

#include <vector>

#include "myShell.h"
/*searchCommand()*/
/*In this function, we check if user's input has '/'*/
/*If has '/', we search in specified directory      */
/*If no '/', we search in paths of pathsvector      */
void myShell::searchCommand() {
  if (strchr(argv[0], '/') == nullptr) {
    //if user's input includes no '/'
    parsePATH();
    cmdName = argv[0];
  }
  else {
    //if use's input includes '/', get path
    char * pathend = strrchr(argv[0], '/');
    char * pathbegin = argv[0];
    cmdName.append(pathend + 1);

    //get path and push it into pathsVector
    std::string pathname;
    pathname.append(pathbegin, pathend - pathbegin);
    //std::cout << "User's path is: " << pathname << std::endl;
    pathsVector.push_back(pathname);
  }
  DIR * ParentPath;
  struct dirent * ent;
  //search command
  for (std::vector<std::string>::iterator it = pathsVector.begin(); it != pathsVector.end(); ++it) {
    ParentPath = opendir((*it).c_str());
    if (ParentPath == nullptr) {
      std::cerr << "Opendir failed!" << std::endl;
      return;
    }
    else {
      //readdir
      while ((ent = readdir(ParentPath)) != nullptr) {
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0)) {
          if (strcmp(ent->d_name, cmdName.c_str()) == 0) {
            //command found
            std::string cmdPath = *it + "/" + cmdName;

            delete[] argv[0];                          //delete old argv[0]
            argv[0] = new char[cmdPath.length() + 1];  //create new argv[0]
            strcpy(argv[0], cmdPath.c_str());          //store full-path-cmd into argv[0]
            closedir(ParentPath);
            cmdFound = 1;
            return;
          }
        }
      }
      closedir(ParentPath);
    }
  }
  std::cout << "Command " << cmdName << " not found" << std::endl;
}
