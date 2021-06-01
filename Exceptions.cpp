#include "Exceptions.h"
        GraphException::GraphException(const std::string& error_type) : error_string("Error: " + error_type) {}

        GraphException::GraphException(const std::string& error_type, const std::string& error_spec) : 
                                    error_string("Error: " + error_type + ": '" + error_spec + "'") {}
        
        const char* GraphException::what() const noexcept{
            return error_string.c_str();
        }

        GraphException::~GraphException() {}

        CannotOpenFile::CannotOpenFile(std::string error_spec) : GraphException("Cannot Open File", error_spec) {}
        InvalidReadFromFile::InvalidReadFromFile(std::string error_spec) : GraphException("Invalid Read From File", error_spec) {}
        NotEnoughInput::NotEnoughInput() : GraphException("Not Enough Input") {}

        IllegalGraphName::IllegalGraphName(std::string error_spec) : GraphException("Illegal Graph Name", error_spec) {}
        IllegalVertexIdUnknownCharacter::IllegalVertexIdUnknownCharacter(std::string error_spec) : GraphException("Illegal Vertex Id - Unknown Character", error_spec) {}
        IllegalVertexIdUnbalancedBrackets::IllegalVertexIdUnbalancedBrackets(std::string error_spec) : GraphException("Illegal Vertex Id - Unbalanced Brackets", error_spec) {}
        IllegalVertexIdSemicolon::IllegalVertexIdSemicolon(std::string error_spec) : GraphException("Illegal Vertex Id - Misplaced Semicolon", error_spec) {}
        EmptyVertex::EmptyVertex() : GraphException("Illegal Vertex Id - Empty Vertex"){}
        VertexAlreadyExists::VertexAlreadyExists(std::string error_spec) : GraphException("Vertex Already Exists", error_spec) {}
        VertexDoesNotExist::VertexDoesNotExist(std::string error_spec) : GraphException("Can't Connect Edge - Vertex Does Not Exist", error_spec){}
        SelfLoop::SelfLoop() : GraphException("Self Loop"){}
        EdgeNotClosed::EdgeNotClosed() : GraphException("Edge Not Closed"){}
        EdgeAlreadyExists::EdgeAlreadyExists(std::string error_spec) : GraphException("Edge Already Exists", error_spec){}

        UnrecognizedCommand::UnrecognizedCommand(std::string error_spec) : GraphException("Unrecognized Command", error_spec){}
        NoCommandGiven::NoCommandGiven() : GraphException("No Command Given After ="){}
        FirstElementIsAnOperator::FirstElementIsAnOperator() : GraphException("Illegal Syntax: First Element Is An Operator"){}
        LastElementIsAnOperator::LastElementIsAnOperator() : GraphException("Illegal Syntax: Last Element In Expression Is An Operator"){}
        UnrecognizedElement::UnrecognizedElement(std::string error_spec) : GraphException("Illegal Syntax: Unrecognized Element", error_spec){}
        NoGraphAfterOperator::NoGraphAfterOperator() : GraphException("Illegal Syntax: No Graph After Operator"){}
        NoOperatorAfterGraph::NoOperatorAfterGraph() : GraphException("Illegal Syntax: No Operator After Graph"){}
        UndefinedVariable::UndefinedVariable(std::string error_spec) : GraphException("Undefined Variable", error_spec){}
        UnknownOperator::UnknownOperator() : GraphException("Unknown Operator"){}

        IllegalFileName::IllegalFileName(std::string error_spec) : GraphException("Illegal File Name", error_spec){}
        InvalidAgrumentsGcalc::InvalidAgrumentsGcalc() : GraphException("Invalid amount of arguments given"){}
        UnbalancedParantheses::UnbalancedParantheses() : GraphException("Unbalanced Parantheses In Expression"){}

        NullPointerGiven::NullPointerGiven() : GraphException("Null Pointer Given"){}
        
