#ifndef GRAPH_H
#define GRAPH_H
#include <iostream> 
#include <iterator> 
#include <map>
#include <vector>
#include <set>

class Graph{
    std::map<std::string, std::set<std::string>> Vertices; //string: vertex id. set of strings: the vertices it's connected to.
    void addVerticesToGraph(std::string vertices_str);
    void addEdgesToGraph(std::string edges_str);
    static bool balancedBrackets(const std::string vertex_id);
    static bool legalSemicolon(const std::string vertex_id);
    static std::string readVertexIdFromFile(std::ifstream &infile, std::string file_name);
    static void writeStringToFile(std::string str, std::ofstream &outfile);

    public:
    Graph(std::vector<std::string> graphVector);
    Graph();
    int getNumEdges() const;
    Graph& operator=(const Graph& other);
    Graph(const Graph&) = default;
    friend Graph operator+(const Graph& graph_1, const Graph& graph_2);
    friend Graph operator^(const Graph& graph_1, const Graph& graph_2);
    friend Graph operator-(const Graph& graph_1, const Graph& graph_2);
    friend Graph operator*(const Graph& graph_1, const Graph& graph_2);
    Graph operator!() const;    
    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);
    void writeGraphToFile(const std::string file_name) const;
    void readGraphFromFile(const std::string file_name);
    static void verifyLegalVertexId(const std::string vertex_id);
    void addVertex(std::string vertex_id);
    void addEdge(std::string source_vertex, std::string dest_vertex);

    ~Graph()=default;
};


#endif