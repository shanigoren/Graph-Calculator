#include "Graph_Interface.h"
#include "Exceptions.h"
#include "Graph.h"

Graph* create(){
    Graph * graph = new Graph();
    return graph;
}

void destroy(Graph* graph){
    delete graph;
}

void disp(Graph* graph){
    std::cout << *graph;
}

Graph* addVertex(Graph* graph, char* vertex){
    try {
        if (graph == nullptr)
        {
            throw NullPointerGiven();
        }
        std::string vertex_str(vertex);
        graph->verifyLegalVertexId(vertex_str);
        graph->addVertex(vertex);
        return graph;
    } catch (std::exception & e){
        std::cout << e.what() << std::endl;
        return nullptr;
    }
}

Graph* addEdge(Graph* graph, char* vertex1, char* vertex2){
    try {
        if (graph == nullptr)
        {
            throw NullPointerGiven();
        }
        std::string vertex1_str(vertex1);
        std::string vertex2_str(vertex2);
        graph->addEdge(vertex1_str, vertex2_str);
        return graph;
    } catch (std::exception & e){
        std::cout << e.what() << std::endl;
        return nullptr;
    }
}

Graph* graphUnion(Graph* graph_in1, Graph* graph_in2, Graph* graph_out){
    try {
        if (graph_in1 == nullptr || graph_in2 == nullptr)
        {
            throw NullPointerGiven();
        }
        *graph_out = *graph_in1 + *graph_in2;
        return graph_out;
    } catch (std::exception & e){
        std::cout << e.what() << std::endl;
        return nullptr;
    }
}

Graph* graphIntersection(Graph* graph_in1,Graph* graph_in2, Graph* graph_out){
    try {
        if (graph_in1 == nullptr || graph_in2 == nullptr)
        {
            throw NullPointerGiven();
        }
        *graph_out = (*graph_in1)^(*graph_in2);
        return graph_out;
    } catch (std::exception & e){
        std::cout << e.what() << std::endl;
        return nullptr;
    }
}

Graph* graphDifference(Graph* graph_in1, Graph* graph_in2, Graph* graph_out){
    try {
        if (graph_in1 == nullptr || graph_in2 == nullptr)
        {
            throw NullPointerGiven();
        }
        *graph_out = (*graph_in1)-(*graph_in2);
        return graph_out;
    } catch (std::exception & e){
        std::cout << e.what() << std::endl;
        return nullptr;
    }
}

Graph* graphProduct(Graph* graph_in1, Graph* graph_in2, Graph* graph_out){
    try {
        if (graph_in1 == nullptr || graph_in2 == nullptr)
        {
            throw NullPointerGiven();
        }
        *graph_out = (*graph_in1)*(*graph_in2);
        return graph_out;
    } catch (std::exception & e){
        std::cout << e.what() << std::endl;
        return nullptr;
    }
}

Graph* graphComplement(Graph *graph_in, Graph* graph_out){
    try {
        if (graph_in == nullptr)
        {
            throw NullPointerGiven();
        }
        *graph_out = !(*graph_in);
        return graph_out;
    } catch (std::exception & e){
        std::cout << e.what() << std::endl;
        return nullptr;
    }
}
