#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <utility>
#include <vector>

#include "myShell.h"
/*This file contains three functions about Variables*/
/*
prsDetCmd(): for set command, we design a particular parsing function
setVar(): create a var or change its value
exportVar():put the value of var into envir for other programs
incVar(): increment val by 1
validatevarcmd(): transforms "$var" to values of var and store them in arg vector
*/

void myShell::prsSetCmd() {
  //similar as parseInput
  arg.clear();
  std::string single_arg;
  int parsed_num = 0;
  for (std::string::iterator it = validInput.begin(); it != validInput.end(); it++) {
    std::string::iterator it2 = it;
    it2++;  //very important, can not write it2=++it, it will break loop!
    if (isspace(*it) == 0) {
      //if current is not space, push it in single_arg
      single_arg.push_back(*it);
      if (parsed_num == 2) {
        single_arg.append(it2, validInput.end());
        arg.push_back(single_arg);
        break;
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
          parsed_num++;
          single_arg.clear();
        }
      }
    }
  }
}
void myShell::setVar() {
  if (arg_n < 2) {
    std::cerr << "set needs 2 or 3 arguments." << std::endl;
    return;
  }
  else {
    prsSetCmd();
    size_t len = strlen(arg[1].c_str());
    //check if invalid varname
    for (size_t i = 0; i < len; i++) {
      if ((isalpha(arg[1][i]) == 0) && (isdigit(arg[1][i]) == 0) && (arg[1][i] != '_')) {
        std::cerr << "Invalid var name\n";
        return;
      }
    }
    std::map<std::string, std::string>::iterator it = VarMap.find(arg[1]);
    if (it != VarMap.end()) {
      it->second = arg_n == 2 ? "" : arg[2];
      //if no value, set "" to variable
    }
    else {
      std::pair<std::string, std::string> newVar;
      newVar = arg_n == 2 ? std::make_pair(arg[1], "") : std::make_pair(arg[1], arg[2]);
      VarMap.insert(newVar);
      //std::cout << "New Var is " << std::get<0>(newVar) << "->" << std::get<1>(newVar) << std::endl;
    }
  }
}
void myShell::exportVar() {
  if (arg_n != 2) {
    std::cerr << "export command need 1 argument\n";
  }
  else {
    std::map<std::string, std::string>::iterator it = VarMap.find(argv[1]);
    if (it != VarMap.end()) {
      //Found var, export it;
      setenv(argv[1], (it->second).c_str(), 1);
    }
  }
}
void myShell::incVar() {
  if (arg_n != 2) {
    std::cerr << "inc needs 1 argument\n";
  }
  else {
    std::map<std::string, std::string>::iterator it = VarMap.find(argv[1]);
    if (it != VarMap.end()) {
      if ((it->second).size() == 0) {
        //if val is unset, set to "1"
        it->second = "1";
      }
      else {
        std::string::iterator sit;
        for (sit = (it->second).begin(); sit != (it->second).end(); ++sit) {
          if (isdigit(*sit) == 0) {
            break;  //break once we find a non-digit
          }
        }
        if (sit == (it->second).end()) {
          //if all digit, transform to "number"
          int num = std::stoi(it->second);
          num++;
          it->second = std::to_string(num);
          //increment it and transform to string
        }
        else {
          //if has any char, set to "1"
          it->second = "1";
        }
      }
    }
    else {
      std::cerr << "Variable " << argv[1] << " not found\n";
    }
  }
}
/*validateVarCmd*/
/*
This function is added for step3.
it transforms "$var" to values of var and store them in arg vector
*/
void myShell::validateVarCmd() {
  for (size_t i = 0; i < arg.size(); i++) {
    std::vector<std::string> varNameList;  //container for all var name with $
    std::string longVarName = arg[i];

    if (longVarName.find('$') == std::string::npos) {
      continue;
    }
    else {
      while (longVarName.find('$') != std::string::npos) {
        //Once we find '$' in longVarName, chack if it is a real varname
        size_t i = longVarName.find('$');
        std::string varName;
        std::string::iterator name_begin = longVarName.begin() + i;
        for (std::string::iterator it = longVarName.begin() + i + 1; it != longVarName.end();
             ++it) {
          //for each char in this $argument
          if ((isalpha(*it) == 0) && (isdigit(*it) == 0) && ((*it) != '_')) {
            //if it not belongs to a var name
            if (*(it - 1) != '$') {
              //if the previous one is not $
              //get a valid var name
              varName.append(name_begin + 1, it);
              varNameList.push_back(varName);
            }
            std::string rest;
            rest.append(it, longVarName.end());
            longVarName.clear();
            longVarName.append(rest);
            break;
          }
          else {
            //if it could be a valid var name
            if (((it + 1) == longVarName.end())) {
              //and next is end
              //get the final var name
              varName.append(name_begin + 1, it + 1);
              varNameList.push_back(varName);
              varName.clear();
              longVarName.clear();
              break;
            }
          }
        }
      }

      std::string final_argi;
      std::string::iterator begin = arg[i].begin();
      for (size_t n = 0; n < varNameList.size(); n++) {
        size_t found = arg[i].find(varNameList[n]);
        if (found == std::string::npos) {
          std::cerr << "Failed to parse $variables\n";
        }
        else {
          arg[i][found] = '0';  //replace '$' with '0'

          //append non-var content
          final_argi.append(begin, arg[i].begin() + found - 1);
          //find var value and append it
          std::map<std::string, std::string>::iterator map_it = VarMap.find(varNameList[n]);
          if (map_it != VarMap.end()) {
            // Found
            final_argi.append(map_it->second);
          }
          else {
            // Not Found
            std::cerr << "Variable $" << varNameList[n] << " Not Found\n";
            final_argi.append("$");
            final_argi.append(varNameList[n]);
          }
          //change the begin position for next loop
          begin = arg[i].begin() + found + varNameList[n].length();
        }
        if (n == varNameList.size() - 1) {
          //if it's the last time loop, append rest constent
          final_argi.append(begin, arg[i].end());
        }
      }
      arg[i].clear();
      arg[i].append(final_argi);
    }
  }
}
