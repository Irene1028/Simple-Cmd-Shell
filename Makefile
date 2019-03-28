myShell: main.cpp myShell.cpp parseInput.cpp parsePATH.cpp searchCommand.cpp validateInput.cpp myShell.h cdCommand.cpp Vars.cpp
	g++ -std=gnu++11 -Wall -Werror -pedantic -o myShell main.cpp parseInput.cpp parsePATH.cpp searchCommand.cpp validateInput.cpp myShell.cpp cdCommand.cpp Vars.cpp
