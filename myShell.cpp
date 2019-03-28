#include "myShell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     //for strcpy
#include <sys/types.h>  //for waitpid
#include <sys/wait.h>   //for waitpid
#include <unistd.h>     //for getcwd

#include <iostream>
#include <vector>

/*Constructor*/
myShell::myShell() :
    exit(false),
    argv(nullptr),
    ECE551PATH(getenv("PATH")),
    arg_n(0),
    cmdFound(0) {}

/*Destructor*/
myShell::~myShell() {}

/*destroyArgv()*/
void myShell::destroyArgv() {
  input.clear();
  validInput.clear();
  arg.clear();
  pathsVector.clear();
  cmdName.clear();
  cmdFound = 0;
  for (int i = 0; i < arg_n; i++) {
    delete[] argv[i];
  }
  delete[] argv;
}
/*exitmyshell()*/
/*set exit as true when we want to exit*/
void myShell::exitmyShell() {
  exit = true;
}

/*getExit()*/
/*return the value of exit*/
bool myShell::getExit() {
  return exit;
}

/*runCommand()*/
/*create a child process to run the command*/
/*call fork();execve();                    */
void myShell::runCommand() {
  //std::cout << "Here is runCommand!" << std::endl;

  pid_t pid;
  pid = fork();
  if (pid < 0) {
    std::cerr << "fork failed." << std::endl;
    exitmyShell();
    //If fork failed, we exit myShell
  }
  if (pid == 0) {
    //In child process

    if (execve(argv[0], argv, environ) == -1) {
      perror("execve");
      exitmyShell();
    }
  }
  if (pid > 0) {
    //In parent process

    int state_val;
    waitpid(pid, &state_val, 0);
    if (WIFEXITED(state_val)) {
      std::cout << "Program exited with status " << WEXITSTATUS(state_val) << std::endl;
    }
    else if (WIFSIGNALED(state_val)) {
      std::cout << "Program was killed by signal " << WTERMSIG(state_val) << std::endl;
    }
  }
}

//runMyshell()
void myShell::runMyShell() {
  fflush(stdout);  //clean stdout
  char cwd[128];
  getcwd(cwd, sizeof(cwd) - 1);            //get a path
  std::cout << "myShell:" << cwd << "$ ";  //print a prompt
  //Here we expect an input
  std::getline(std::cin, input);
  if (std::cin.eof() || std::cin.fail()) {
    //fail to input
    exitmyShell();
    std::cerr << "Failed to get input" << std::endl;
  }
  else {
    //success cin
    validateInput();  //get a valid input line without escaped space
    if (validInput.size() == 0) {
      //if validinput is null, exit immediately
      std::cerr << "Input should not be null" << std::endl;
    }
    else {
      //if valid input is not null
      //parse input and get an array argv[]
      arg_n = parseInput();  //parseInput.cpp
      if (strcmp(argv[0], "exit") == 0) {
        //if command is exit
        exitmyShell();
      }
      else if (strcmp(argv[0], "cd") == 0) {
        //if command is cd, call cdCommand func
        cdCommand();  //cdCommand.cpp
      }
      else if (strcmp(argv[0], "set") == 0) {
        setVar();  //Vars.cpp
      }
      else if (strcmp(argv[0], "export") == 0) {
        exportVar();  //Vars.cpp
      }
      else if (strcmp(argv[0], "inc") == 0) {
        incVar();  //Vars.cpp
      }
      else {
        //search the command and run it
        searchCommand();
        if (cmdFound == 1) {
          runCommand();
        }
      }
      //destroy argv[] for next loop
      destroyArgv();
    }
  }
}
