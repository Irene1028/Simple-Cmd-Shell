#include <string.h>

#include "myShell.h"

/*validateInput()*/
/*This function used to validate raw input, which might have '\ 'in it */
/*It should produce a good input with only arguments and spaces        */
/*In this validateInput, '\ 'should be represented with '*'            */

void myShell::validateInput() {
  std::string pathContainer;
  validInput.clear();
  for (std::string::iterator it = input.begin(); it != input.end(); ++it) {
    std::string::iterator it2 = it + 1;
    if (*it != '\\' || (*it == '\\' && isspace(*it2) == 0)) {
      //if current char is not slash
      //or current is slash and next is not space
      //we put it into container.
      pathContainer.push_back(*it);
    }
    else {
      //if current char is slash ,and next char is space
      //we make it++ to jump across the escaped space
      pathContainer.push_back('\0');
      it++;
    }
  }
  validInput.append(pathContainer);
}
