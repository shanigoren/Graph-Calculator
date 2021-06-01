
%module graph
%{
    #include "Graph_Interface.h"
%}

Graph* create();
void destroy(Graph* graph);
void disp(Graph* graph);
Graph* addVertex(Graph* graph, char* vertex);
Graph* addEdge(Graph* graph, char* v1, char* v2);
Graph* graphUnion(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphIntersection(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphDifference(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphProduct(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphComplement(Graph* graph_in, Graph* graph_out);
