#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <string>
#include <vector>

namespace CommandParser{
    void parseCommand(std::string command_line, std::vector<std::string>& CommandVector);
    std::string cleanFirstLastSpaces(std::string command_line);
    bool isGraphName(std::string str);
    bool isGraph(std::string str);
    bool isOperator(char c);
    std::string adjustFunctionSyntax(std::string command_line, std::string func_name);  
    void loadVector(std::vector<std::string> &RightSideVector, std::string right_side);
    void verifyLegalVector(std::vector<std::string> &inputVector);
}

#endif