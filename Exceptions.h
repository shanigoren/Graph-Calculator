#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
#include <string>

    class Exception : public std::exception {};

    class GraphException : public Exception{
        std::string error_string;
        std::string error_spec;
        public:
        GraphException(const std::string& error_type);
        GraphException(const std::string& error_type, const std::string& error_spec);
        const char* what() const noexcept;
        virtual ~GraphException();
    };
    
    class CannotOpenFile : public GraphException {
        public:
        CannotOpenFile(std::string error_spec);
    };
    class InvalidReadFromFile : public GraphException {
        public:
        InvalidReadFromFile(std::string error_spec);
    };
    class NotEnoughInput : public GraphException {
        public:
        NotEnoughInput();
    };
    

    class IllegalGraphName : public GraphException {
        public:
        IllegalGraphName(std::string error_spec);
    };
    class IllegalVertexIdUnknownCharacter : public GraphException {
        public:
        IllegalVertexIdUnknownCharacter(std::string error_spec);
    };
    class IllegalVertexIdUnbalancedBrackets : public GraphException {
        public:
        IllegalVertexIdUnbalancedBrackets(std::string error_spec);
    };
    class IllegalVertexIdSemicolon : public GraphException {
        public:
        IllegalVertexIdSemicolon(std::string error_spec);
    };
    class VertexAlreadyExists : public GraphException {
        public:
        VertexAlreadyExists(std::string error_spec);
    };
    class EmptyVertex : public GraphException {
        public:
        EmptyVertex();
    };
    class SelfLoop : public GraphException {
        public:
        SelfLoop();
    };
    class EdgeNotClosed : public GraphException {
        public:
        EdgeNotClosed();
    };
    class VertexDoesNotExist : public GraphException {
        public:
        VertexDoesNotExist(std::string error_spec);
    };
    class EdgeAlreadyExists : public GraphException {
        public:
        EdgeAlreadyExists(std::string error_spec);
    };

    class UnrecognizedCommand : public GraphException {
        public:
        UnrecognizedCommand(std::string error_spec);
    };
    class NoCommandGiven : public GraphException {
        public:
        NoCommandGiven();
    };
    class FirstElementIsAnOperator : public GraphException {
        public:
        FirstElementIsAnOperator();
    };
    class LastElementIsAnOperator : public GraphException {
        public:
        LastElementIsAnOperator();
    };
    class UnrecognizedElement : public GraphException {
        public:
        UnrecognizedElement(std::string error_spec);
    };
    class NoGraphAfterOperator : public GraphException {
        public:
        NoGraphAfterOperator();
    };
    class NoOperatorAfterGraph : public GraphException {
        public:
        NoOperatorAfterGraph();
    };
    class UndefinedVariable : public GraphException {
        public:
        UndefinedVariable(std::string error_spec);
    };
    class UnknownOperator : public GraphException {
        public:
        UnknownOperator();
    };
    
    class IllegalFileName : public GraphException {
        public:
        IllegalFileName(std::string error_spec);
    };

    class InvalidAgrumentsGcalc : public GraphException {
        public:
        InvalidAgrumentsGcalc();
    };

    class UnbalancedParantheses : public GraphException {
        public:
        UnbalancedParantheses();
    };

    class NullPointerGiven : public GraphException {
        public:
        NullPointerGiven();
    };
    
#endif