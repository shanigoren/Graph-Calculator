#include "CommandHandler.h"
#include "Exceptions.h"
#include <iostream> 
#include <cctype>
#include <string>
#include <vector>
#include <iterator>
#include <stack>
using namespace CommandParser;

bool CommandParser::isOperator(char c){
    std::vector<char> SpecialChars {' ', '+', '^', '-', '*'};
    for (auto const &s: SpecialChars){
        if (c == s){
            return true;
        }
    }
    return false;
}

static bool isValidFunctionSyntax(std::string command_line, std::string func_name){
    return command_line == func_name;
}

std::string CommandParser::cleanFirstLastSpaces(std::string command_line){
    std::size_t first_char = command_line.find_first_not_of(" ");
    std::size_t last_char = command_line.find_last_not_of(" \t\f\v\n\r");
    std::string whitespaces (" \t\f\v\n\r");
    std::size_t found = command_line.find_last_not_of(whitespaces);
    if (found!=std::string::npos)
        command_line.erase(found+1);
    else //line is all spaces
        command_line.clear();
    if (!command_line.empty())
        command_line = command_line.substr(first_char,last_char+1);
    //std::cout << "command line w/out spaces:-" << command_line << "-" << std::endl;
    return command_line;
}


std::string CommandParser::adjustFunctionSyntax(std::string command_line, std::string func_name){
    //check if last char is ")" - if not, throw error
    std::size_t last_p = command_line.find_last_of(")");
    if (last_p != command_line.length()-1){
        throw UnrecognizedCommand(command_line);
    }
    //remove command word
    command_line = command_line.substr(func_name.length(),command_line.length()-func_name.length()-1); 
    //remove last parentheses:
    command_line = command_line.substr(0,command_line.length());
    //make sure nothing but spaces before "("
    std::size_t first_p = command_line.find_first_of("(");  
    for (size_t i = 0; i<first_p; i++){
        if (command_line[i] != ' '){
            throw UnrecognizedCommand(command_line);
        }
    }
    //remove first parentheses
    command_line.erase(first_p,1);
    //remove first and last spaces (those between parentheses and expression)
    command_line = CommandParser::cleanFirstLastSpaces(command_line);
    return command_line;
}

static std::string parseParentesesExpression(std::string right_side){
    std::string current_string = "";
    std::stack<size_t> index_stack;
    right_side = cleanFirstLastSpaces(right_side);
    for (size_t i=0; i<right_side.length(); i++) {
        current_string += right_side[i];
        if (right_side[i] == '(') {
            index_stack.push(i);
        }
        else if (right_side[i] == ')') {
            index_stack.pop();
            if (index_stack.empty()) {
                return current_string;
            }
        }
    }
    throw UnbalancedParantheses();
}

static std::string parseLoadElement(std::vector<std::string> &RightSideVector, std::string right_side, size_t &i){
    std::size_t find = right_side.find(")",i);
    if (find == std::string::npos){
        return "";
    }
    std::string current_str = right_side.substr(i,find-i+1);
    i += current_str.length()-1;
    return current_str;
}

static std::string parseComplementElement(std::vector<std::string> &RightSideVector, std::string right_side, size_t &i){
    std::string current_str = right_side.substr(i,right_side.length());
    int count = 0;
    for (size_t j=0; j < current_str.length(); j++){
        if (current_str[j] == ' ') {
            i++;
            continue;
        }
        if (current_str[j] == '!') {
            i++;
            count++;
        }
        else {
            // The outer loop in the loadVector method increments i by 1.
            i--;
            break;
        }
    }
    return (count%2==0) ? "" : "!";
}

void CommandParser::loadVector(std::vector<std::string> &RightSideVector, std::string right_side){
    std::string current_str;
    right_side = cleanFirstLastSpaces(right_side);
    if (right_side.empty())
        throw NoCommandGiven();
    const std::string load = "load";
    for (size_t i=0; i < right_side.length(); i++){
        if (right_side[i] == '('){
            std::string parentheses_exp = parseParentesesExpression(right_side.substr(i, right_side.length()));
            i += parentheses_exp.length() - 1;
            current_str = current_str + parentheses_exp;
            RightSideVector.push_back(current_str);
            current_str.clear();
            continue;
        }
        // 1. ! Complement
        if (right_side[i] == '!'){
            std::string compStr = parseComplementElement(RightSideVector, right_side, i);
            current_str = compStr; 
            continue;
        }
        // 2. load command
        if (right_side.compare(i, load.length(), load) == 0){ 
            current_str = current_str + parseLoadElement(RightSideVector, right_side, i);
            if (!current_str.empty()){
                RightSideVector.push_back(current_str);
                current_str.clear();
                continue;
            }
        }
        if (right_side[i] == '{'){
            if (current_str!="!" && !current_str.empty()){
                RightSideVector.push_back(current_str);
                current_str.clear();
            }
            while (i < right_side.length() && right_side[i] != '}')
                current_str += right_side[i++];
            current_str += right_side[i];
            if (!current_str.empty()){
                RightSideVector.push_back(current_str);
                current_str.clear();
            }
            continue;
        }
        else if (right_side[i] == ' '){
            if (!current_str.empty()){
                RightSideVector.push_back(current_str);
                current_str.clear();
            }
            continue;
        }
        else if (!CommandParser::isOperator(right_side[i])){
            current_str += right_side[i];
        }
        else {
            if (!current_str.empty())
                RightSideVector.push_back(current_str);
            std::string opr;
            opr += right_side[i];
            RightSideVector.push_back(opr);
            current_str.clear();
        }
    }
    if (!current_str.empty())
        RightSideVector.push_back(current_str);
}


static void recursivelyHandleParentesesExpression(std::vector<std::string> &inputVector){
    // bool hasParanthesis = false;
    std::vector<std::string> resultVector;
    for (std::vector<std::string>::const_iterator i=inputVector.begin(); i!=inputVector.end();i++){
        std::string element = *i;
        if (element[0] == '('){
            // inputVector.erase(i);
            // hasParanthesis = true;
            //remove () from expression

            //make new vector from this element
            std::vector<std::string> ParanthesesResultVector;
            std::string expresion_to_load = element.substr(1, element.length() - 2);
            expresion_to_load = cleanFirstLastSpaces(expresion_to_load);
            if (expresion_to_load[0])
            loadVector(ParanthesesResultVector, expresion_to_load);
            recursivelyHandleParentesesExpression(ParanthesesResultVector);
            // inputVector.reserve(inputVector.size() + ParanthesesResultVector.size());
            // inputVector.insert(i, ParanthesesResultVector.begin(), ParanthesesResultVector.end());

            for (std::vector<std::string>::iterator elementToCopy=ParanthesesResultVector.begin(); elementToCopy!=ParanthesesResultVector.end();++elementToCopy) {
                resultVector.push_back(*elementToCopy);
            }
        }
        else {
            resultVector.push_back(element);
        }
    }
    inputVector = resultVector;
    // if (!hasParanthesis) {
        // Replace ParanthesesResultVector RightSideVector's element with the new vector elements
        
    // }
}

void CommandParser::verifyLegalVector(std::vector<std::string> &inputVector){
    // vector must contain at least one element
    std::vector<std::string> inputVectorCopy;
    for (std::vector<std::string>::iterator element=inputVector.begin(); element!=inputVector.end();++element) {
        inputVectorCopy.push_back(*element);
    }
    if (inputVectorCopy.empty()){
        throw NoCommandGiven();
    }
    // 1. The first element must not be an operator
    char first_element = inputVectorCopy[0][0];
    if (CommandParser::isOperator(first_element)){
        throw FirstElementIsAnOperator();
    }
    // 2. The last element must not be an operator
    char last_element = inputVectorCopy[inputVectorCopy.size()-1][0];
    if (CommandParser::isOperator(last_element)){
        throw LastElementIsAnOperator();
    }
    if (!CommandParser::isGraph(inputVectorCopy[inputVectorCopy.size()-1]) && (last_element != '(')){
        throw UnrecognizedElement(inputVectorCopy[inputVectorCopy.size()-1]);
    }
    // reaching here, we know that first and last elements are not operators
    // 3.  Between each element, there must be an operator
    for(size_t i = 0; i+1 != inputVectorCopy.size(); i++) {
        //possible legal cases:
        // 0. element i is () expression 
        if ((inputVectorCopy[i]).empty())
            throw NoCommandGiven();
        if (inputVectorCopy[i][0] == '('){
            continue;
        }
        // 1. element i is a graph - after it must come an operator 
        else if (CommandParser::isGraph(inputVectorCopy[i])){
            if (CommandParser::isOperator(inputVectorCopy[i+1][0]))
                continue;
            else   
                throw NoOperatorAfterGraph();
        }
        // 2. element i is an operator - after it must come a graph or '!'
        else if (CommandParser::isOperator(inputVectorCopy[i][0])){
            // after operator must be a graph
            if (CommandParser::isGraph(inputVectorCopy[i+1]) || inputVectorCopy[i+1][0] == '(')
                continue;
            else{
                throw NoGraphAfterOperator();
            }
        }
        else{
            throw UnrecognizedElement(inputVectorCopy[i]);
        }
    }    
}

static void loadExpressionToVector(std::string expression_str, std::vector<std::string>& CommandVector){
    std::vector<std::string> ExpVector;
    loadVector(ExpVector, expression_str);
    verifyLegalVector(ExpVector);
    // cat the expression vector to command vector
    CommandVector.insert(CommandVector.end(), ExpVector.begin(), ExpVector.end());   
}

static void verifyLegalFileName(std::string file_name){
    for (char const &c: file_name){
        if (c == ',' || c == ')' || c =='('){
            throw IllegalFileName(file_name);
        }
    }    
}

static bool isVariableFunction(std::string func, std::string command_line, std::vector<std::string>& CommandVector) {
    std::size_t last_p = command_line.find_last_of(")");
    if (last_p != command_line.length()-1){
        return false;
    }
    //remove command word
    command_line = command_line.substr(func.length(),command_line.length()-func.length()-1); 
    //make sure nothing but spaces before "("
    std::size_t first_p = command_line.find_first_of("(");
    for (size_t i = 0; i<first_p; i++){
        if (command_line[i] != ' '){
            return false;
        }
    }
    return true;
}

static void handleVariableFunction(std::string func, std::string command_line, std::vector<std::string>& CommandVector) {
    if (func == "print"){
        std::string expression_str = CommandParser::adjustFunctionSyntax(command_line, func);
        CommandVector.push_back(func);
        loadExpressionToVector(expression_str, CommandVector);                
    }
    else if (func == "save"){
        std::string exp_and_file = CommandParser::adjustFunctionSyntax(command_line, func);
        //find comma - if there isn't one throw exception
        int comma = exp_and_file.find_last_of(","); //changed this - maybe bug
        if (comma == -1){
            throw UnrecognizedCommand(command_line);
        }
        //maybe bug - check indexes again!!!
        std::string expression_str = exp_and_file.substr(0,comma);
        std::string file_name = cleanFirstLastSpaces(exp_and_file.substr(comma+1, exp_and_file.length()-comma-1));
        //verify legal file name 
        verifyLegalFileName(file_name);
        // 1. expression - load to vector (exactly like print)
        CommandVector.push_back(func);
        loadExpressionToVector(expression_str, CommandVector);
        // 2. file name (assuming validity)
        CommandVector.push_back(file_name);              
    }
    else if (func == "delete"){
        //this has to be a defined variable
        std::string variable = CommandParser::adjustFunctionSyntax(command_line, func);
        if (!CommandParser::isGraphName(variable)){
            throw IllegalGraphName(variable);
        }
        CommandVector.push_back(func);
        CommandVector.push_back(variable);
    }
}

static void handleSpecialFunctions(std::string command_line, std::vector<std::string>& CommandVector){
    std::vector<std::string> protectedNames {"print", "delete","save","who", "reset"};
    for (std::vector<std::string>::iterator func=protectedNames.begin(); func!=protectedNames.end();++func){
        if (command_line.compare(0, (*func).length(), *func) == 0){
            if (*func == "who" || *func == "reset"){
                // make sure the command is the only string in the command line
                if (isValidFunctionSyntax(command_line, *func))
                    CommandVector.push_back(*func);
            }
            else if (*func == "print" || *func == "save" || *func == "delete") {
                if (!isVariableFunction(*func, command_line, CommandVector))
                    continue;
                handleVariableFunction(*func, command_line, CommandVector);
            }
        }            
    }
}




static void handleAssignmentCommands(std::string command_line, std::vector<std::string>& CommandVector){
    // Find index of char "=" and split it - the left should contain the graph name (and maybe some spaces to the right)
    std::size_t equal_pos = command_line.find("=");
    if (equal_pos == std::string::npos){
        throw UnrecognizedCommand(command_line);
    }
    std::string variable = cleanFirstLastSpaces(command_line.substr(0,equal_pos));
    if (!isGraphName(variable)){
        throw IllegalGraphName(variable);
    }
    std::string right_side = command_line.substr(equal_pos+1,command_line.length()-equal_pos-1);    
    std::vector<std::string> RightSideVector;
    loadVector(RightSideVector, right_side);
    // if first char in element is (
    
    verifyLegalVector(RightSideVector);
    CommandVector.push_back(variable);
    CommandVector.insert(CommandVector.end(), RightSideVector.begin(), RightSideVector.end());
}

void CommandParser::parseCommand(std::string command_line, std::vector<std::string>& CommandVector){
    command_line = CommandParser::cleanFirstLastSpaces(command_line);
    handleSpecialFunctions(command_line, CommandVector);
    if (CommandVector.empty())
        handleAssignmentCommands(command_line, CommandVector);
}

bool CommandParser::isGraphName(std::string str){
    std::vector<std::string> protectedNames {"print", "delete","save","who", "reset","load","quit"};
    for (std::vector<std::string>::iterator func=protectedNames.begin(); func!=protectedNames.end();++func){
        if (str == *func){
            return false;
        }    
    }
    if (!isalpha(str[0])){
        return false;
    }
    for (int i=1; i < int(str.length()); i++){
        if (!isalnum(str[i]))
            return false;
    }   
    return true;
}

bool CommandParser::isGraph(std::string str){
    //possible cases:
    //load from file - still treat it as a graph. valid syntax: parentheses and legal file name
    const std::string load = "load", cload="!load";
    if ((str.compare(0, load.length(), load) == 0) || (str.compare(0, cload.length(), cload) == 0)){
        return true;
    }
    // 1. must 0 with { and end with }
    if (str[0] == '{' && str[str.length()-1] == '}')
        return true;
    if (str.length()>=2 && str[0] == '!' && str[1] == '{' && str[str.length()-1] == '}')
        return true;
    if (str[0] == '!' && isGraphName(str.substr(1,str.length()-1)))
        return true;
    if (str.find("(")!=std::string::npos)
        return true;
    return isGraphName(str);
}