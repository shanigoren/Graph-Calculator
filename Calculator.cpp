#include "Calculator.h"
#include "Graph.h"
#include "Exceptions.h"
#include "CommandHandler.h"
#include <iostream> 
#include <fstream>
#include <map> 
#include <string>
#include <vector>
using namespace CommandParser;
//using namespace CommandExecutor;

Calculator::Calculator(){
    std::map<std::string,Graph> Variables;
}

Graph Calculator::getVariable(const std::string graph_name) const{
    if (!Variables.count(graph_name)){
        throw UndefinedVariable(graph_name);
    }
    return Variables.at(graph_name);
}

void Calculator::setVariable(const std::string graph_name, Graph graph){
    Variables[graph_name] = graph;
}

Graph Calculator::createGraphFromExpression(std::string graph_str) {
    // 1. create graph from saved file. "load(file_name)"
    Graph resultGraph;
    std::string load = "load";
    if (graph_str.compare(0, load.length(), load) == 0){
        //get file name:
        std::string file_name = adjustFunctionSyntax(graph_str, load); //make sure this works as planned
        //call read function that returns a graph
        resultGraph.readGraphFromFile(file_name);
    }
    // 2. create graph from {} expression
    //syntax checked. either : {a,b|<a,b>} or {a,b}
    else {
        std::vector<std::string> graphVector;
        std::size_t found = graph_str.find("|");
        if (found == std::string::npos){
            graphVector.push_back(graph_str.substr(1,graph_str.length()-2));
        }
        else{
            graphVector.push_back(graph_str.substr(1,found-1));
            graphVector.push_back(graph_str.substr(found+1,graph_str.length()-found-2));
        }
        Graph newGraph(graphVector);
        resultGraph = newGraph;
    }
    return resultGraph;
}

Graph Calculator::calculateGraph(const Graph resultGraph, const char currentOperator, const Graph currentGraph) {
    switch (currentOperator){
        case '+': return resultGraph+currentGraph;
        case '^': return resultGraph^currentGraph;
        case '-': return resultGraph-currentGraph; 
        case '*': return resultGraph*currentGraph; 
        default: throw UnknownOperator();
    }
}

Graph Calculator::getGraphFromString(std::string graph_str){
    Graph resultGraph;
    bool isComplement = false;
    isComplement = (graph_str[0] == '!');
    if (isComplement) {
        graph_str = graph_str.erase(0,1);
    }  
    if (CommandParser::isGraphName(graph_str)){
        //handle known graph   
        resultGraph = getVariable(graph_str);
    }  
    else if (CommandParser::isGraph(graph_str)) {
        //handle new graph 
        resultGraph = createGraphFromExpression(graph_str);
    }
    //creating graph from expression failed - error!!
    else if (!CommandParser::isGraphName(graph_str))
        throw IllegalGraphName(graph_str);
    if (isComplement){
        resultGraph = !resultGraph;
    }
    return resultGraph;
}

void Calculator::executeNonVarFuncs(std::vector<std::string>& CommandVector, std::ostream& output){
    if (CommandVector[0] == "who"){
        if (Variables.empty()){
            return;
        }
        for (auto const& graph : Variables){
            output << graph.first << std::endl;
        }
    }
    else if (CommandVector[0] == "reset"){
        Variables.clear();
    }
    else {
        throw UnrecognizedCommand(CommandVector[0]);
    }
}


void Calculator::executeVarFuncs(std::vector<std::string>& CommandVector, std::ostream& output){
    Graph resultGraph;
    if (CommandVector[0] == "delete"){
        if (CommandVector.size()>2){
            //error
        }
        getVariable(CommandVector[1]);
        Variables.erase(CommandVector[1]);
    }
    else if (CommandVector[0] == "print"){
        resultGraph = getGraphFromVector(CommandVector);
        output << resultGraph;
    }
    //save command: 0-save, graphs & operators... , size()-1:filename 
    else if (CommandVector[0] == "save"){
        std::string output_file = CommandVector.back();
        CommandVector.pop_back();
        resultGraph = getGraphFromVector(CommandVector);
        //now actually save resultGraph to File
        resultGraph.writeGraphToFile(output_file);
    }
}

bool Calculator::isComplement(std::string& exp_str){
    bool res = (exp_str[0] == '!');
    if (res) {
        exp_str = exp_str.substr(1,exp_str.length()-1);
        exp_str = cleanFirstLastSpaces(exp_str);
    }
    return res;
}

//recieves a command vector and returns a graph
Graph Calculator::getGraphFromVector(std::vector<std::string>& CommandVector){
    Graph resultGraph;
    Graph currentGraph;
    bool is_complement = false;
    char currentOperator;
    for (std::size_t i=1; i<CommandVector.size(); i++){
        if (i==1){
            is_complement = isComplement(CommandVector[i]);
            if (CommandVector[i][0] == '(') {
                resultGraph = handleParentheses(CommandVector[i]);
                if (is_complement)
                    resultGraph = !resultGraph;
            }
            else {
                resultGraph = getGraphFromString(CommandVector[i]);
                if (is_complement)
                    resultGraph = !resultGraph;
            }
        }
        else if (i%2==0){
            currentOperator = CommandVector[i][0];
        }
        else{
            is_complement = isComplement(CommandVector[i]);
            if (CommandVector[i][0] == '(') {
                currentGraph = handleParentheses(CommandVector[i]);
                if (is_complement)
                    currentGraph = !currentGraph;
            }
            else {
                currentGraph = getGraphFromString(CommandVector[i]);
                if (is_complement) 
                    currentGraph = !currentGraph;
            }
            resultGraph = calculateGraph(resultGraph, currentOperator, currentGraph);
        }  
    }
    return resultGraph;
}

Graph Calculator::handleParentheses(std::string exp_str) {
    // Remove parenthesis
    std::string parentheses_exp = exp_str.substr(1, exp_str.length() - 2);
    std::vector<std::string> parentheses_exp_vector;
    loadVector(parentheses_exp_vector, parentheses_exp);
    if (parentheses_exp_vector.size() == 0)
        throw NoCommandGiven();
    verifyLegalVector(parentheses_exp_vector);
    parentheses_exp_vector.insert(parentheses_exp_vector.begin(),".");
    return getGraphFromVector(parentheses_exp_vector);
}

void Calculator::executeAssignmentCommand(std::vector<std::string>& CommandVector){
    std::string graphName = CommandVector[0];
    Graph resultGraph;
    resultGraph = getGraphFromVector(CommandVector);
    setVariable(graphName, resultGraph);
}

bool Calculator::isVarFunc(std::string command){
    std::vector<std::string> VarFunctions {"print","delete","save"};
    for (std::vector<std::string>::iterator func=VarFunctions.begin(); func!=VarFunctions.end();++func){
        if (command == *func){
            return true;
        }
    }
    return false;
}

void Calculator::executeCommand(std::vector<std::string>& CommandVector, std::ostream& output){
    //case: who, reset
    if (CommandVector.size() == 1){
        executeNonVarFuncs(CommandVector, output);
    }
    //in case of print/delete/save/load
    else if (CommandVector.size() >= 2 && isVarFunc(CommandVector[0])){
        executeVarFuncs(CommandVector, output);  
    }
    //else: assignment commands: a = b + c
    else{
        executeAssignmentCommand(CommandVector);
    }    
}

void Calculator::OperateShell(){
    std::string command_line;
    do{
        try{
            std::cout << "Gcalc> ";
            std::getline(std::cin, command_line);
            if (CommandParser::cleanFirstLastSpaces(command_line) == "quit")
                break;
            if (CommandParser::cleanFirstLastSpaces(command_line).empty())  
                continue;
            std::vector<std::string> CommandVector;
            parseCommand(command_line, CommandVector);
            executeCommand(CommandVector, std::cout);
            }
        catch (GraphException & e) {
            std::cout << e.what() << std::endl;
            }
    }while (!std::cin.eof());
}

void Calculator::OperateBatch(std::string input_file_name, std::string output_file_name){
    std::string command_line;
    std::ifstream input_file(input_file_name);
    std::ofstream output_file(output_file_name);
    if (!input_file) {
        throw CannotOpenFile(input_file_name);
    }
    if (!output_file) {
        throw CannotOpenFile(output_file_name);
    }
    try{    
        do{
            try{
                std::getline(input_file, command_line);
                if (CommandParser::cleanFirstLastSpaces(command_line) == "quit")
                    break;
                if (CommandParser::cleanFirstLastSpaces(command_line).empty())  
                    continue;
                std::vector<std::string> CommandVector;
                parseCommand(command_line, CommandVector);
                executeCommand(CommandVector, output_file);
                }
            catch (GraphException & e) {
                output_file << e.what() << std::endl;
            }
        }while (!input_file.eof());
    }
    catch (std::exception & e)                 
    {                                          
        output_file << e.what() << std::endl;         
    }
}