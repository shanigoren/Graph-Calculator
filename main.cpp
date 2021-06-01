#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <iterator>
//#include "Graph.h"
#include "Exceptions.h"
#include "Calculator.h"
using std::endl;
using std::cout;

int main(int argc, char *argv[]) {
    

    try{
        Calculator gcalc;
        if (argc == 1) {
            gcalc.OperateShell();
        }
        else if (argc == 3){
            gcalc.OperateBatch(argv[1],argv[2]);
        }
        else{
            throw InvalidAgrumentsGcalc();
        }
    }
    catch (std::exception & e)                 
    {                                          
        std::cerr << "Fatal error has occurred:" << e.what() << endl;         
    }
}

