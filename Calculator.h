#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "Graph.h"
#include "CommandHandler.h"
#include <iostream> 
#include <iterator> 
#include <map>
#include <set>

class Calculator{
    std::map<std::string, Graph> Variables; 
    
    
    void executeCommand(std::vector<std::string>& CommandVector, std::ostream& output);
    void executeNonVarFuncs(std::vector<std::string>& CommandVector, std::ostream& output);
    void executeVarFuncs(std::vector<std::string>& CommandVector, std::ostream& output);
    void executeAssignmentCommand(std::vector<std::string>& CommandVector);
    Graph getGraphFromVector(std::vector<std::string>& CommandVector);
    Graph getGraphFromString(std::string graph_str);
    Graph handleParentheses(std::string exp_str);
    static Graph createGraphFromExpression(std::string graph_str);
    static Graph calculateGraph(const Graph resultGraph, const char currentOperator, const Graph currentGraph);
    static bool isVarFunc(std::string command);
    static bool isComplement(std::string& exp_str);
    void writeGraphToFile(Graph graph, std::string file_name);

    public:
    Calculator();
    void OperateShell();
    void OperateBatch(std::string input_file_name, std::string output_file_name);
    Graph getVariable(const std::string graph_name) const;
    void setVariable(const std::string graph_name, Graph graph);

    ~Calculator()=default;
};


#endif