#include "Graph.h"
#include "Exceptions.h"
#include "CommandHandler.h"
#include <iostream> 
#include <fstream>
#include <iterator> 
#include <stdbool.h>
#include <map> 
#include <set>
#include <vector>
#include <stack>
#include <cctype>
#include <string>
using namespace CommandParser;

Graph::Graph()  {
    std::map<std::string, std::set<std::string>> Vertices;
}

int Graph::getNumEdges() const{
    int num_edges = 0;
    for (auto it = Vertices.begin(); it!=Vertices.end(); ++it){
        for (auto internal_it = (*it).second.begin(); internal_it != (*it).second.end(); internal_it++){
            num_edges++;
        }
    }
    return num_edges;
}

Graph::Graph(std::vector<std::string> graphVector)
{
    std::string vertices_str;
    std::string edges_str;
    if (graphVector.size() > 0)
        addVerticesToGraph(graphVector[0]);
    if (graphVector.size() > 1) //if not, there are only vertices and no edges in the graph
        addEdgesToGraph(graphVector[1]);
}

void Graph::addVerticesToGraph(std::string vertices_str){
    //create vertices: string for example: a,b
    //create only vertices in the graph, with empty sets for edges (to be filled later)
    int start = 0, comma = 0;
    vertices_str = CommandParser::cleanFirstLastSpaces(vertices_str);
    if (vertices_str.empty()){
        //no vertices, can return
        return;
    }
    while (comma != -1){
        comma = vertices_str.find(",",start);
        std::string vertex_id = vertices_str.substr(start,comma-start);
        addVertex(vertex_id);
        start = comma+1;
    }
}

void Graph::addEdgesToGraph(std::string edges_str){
//fill edges set. string for example: <x1,x2>,   <x2, x3> , <x3,x1>
    edges_str = CommandParser::cleanFirstLastSpaces(edges_str);
    int start = 0, end=0;
    if (edges_str.empty())
        return;
    //make sure string starts with < and ends with >
    if (edges_str[0] != '<' || edges_str[edges_str.length()-1] != '>'){
        throw EdgeNotClosed(); 
    }
    while(start != -1){            
        //make sure that from closing ">" to next opening "<" is exactly one comma and some spaces:
        if (start != 0){
            int count_commas = 0;
            for (int i=end+1; i < start; i++){
                if (edges_str[i] == ',')
                    count_commas++;
                if (edges_str[i] != ' ' && edges_str[i] != ',')
                    throw EdgeNotClosed();
            }
            if (!count_commas || count_commas > 1)
                throw EdgeNotClosed();
        }
        int internal_comma = edges_str.find(",",start);
        end = edges_str.find(">",start);
        std::string source_vertex = edges_str.substr(start+1,internal_comma-start-1);
        std::string dest_vertex = edges_str.substr(internal_comma+1,end-internal_comma-1);
        addEdge(source_vertex,dest_vertex);
        start = edges_str.find("<",start+1);  
    }
    if (end<int(edges_str.length()-1)){
        throw EdgeNotClosed(); 
    }
}

void Graph::addVertex(std::string vertex_id){
    vertex_id = CommandParser::cleanFirstLastSpaces(vertex_id);
    verifyLegalVertexId(vertex_id);
    if (Vertices.count(vertex_id)){
        throw VertexAlreadyExists(vertex_id);
    }
    std::set<std::string> Edges;
    Vertices[vertex_id] = Edges;
}

void Graph::addEdge(std::string source_vertex, std::string dest_vertex){
    source_vertex = CommandParser::cleanFirstLastSpaces(source_vertex);
    dest_vertex = CommandParser::cleanFirstLastSpaces(dest_vertex);
    std::map<std::string, std::set<std::string>>::iterator src_pos = Vertices.find(source_vertex);
    std::map<std::string, std::set<std::string>>::iterator dst_pos = Vertices.find(dest_vertex);
    if (src_pos == Vertices.end()){
        throw VertexDoesNotExist(source_vertex);
    }
    else if (dst_pos == Vertices.end()){
        throw VertexDoesNotExist(dest_vertex);
    }
    if (source_vertex == dest_vertex){
        throw SelfLoop();
    }
    //find if edge already exists
    std::set<std::string>::iterator edgeto = src_pos->second.find(dest_vertex);
    if (edgeto != src_pos->second.end()){
        throw EdgeAlreadyExists("<"+source_vertex+","+dest_vertex+">");
    }
    src_pos->second.insert(dest_vertex);
}

void Graph::verifyLegalVertexId(std::string vertex_id){
    //recognized an illegal character: not letters, numbers, brackets or ;
    if (vertex_id.empty()){
        throw EmptyVertex();
    }
    for (char const &c: vertex_id){
        if ((!isalnum(c)) && (c != '[') && (c != ']') && (c != ';')){
            throw IllegalVertexIdUnknownCharacter(vertex_id);
        }
    }
    if (!balancedBrackets(vertex_id)){
        throw IllegalVertexIdUnbalancedBrackets(vertex_id);
    }
    if (!legalSemicolon(vertex_id)){
        throw IllegalVertexIdSemicolon(vertex_id);
    }    
}

bool Graph::balancedBrackets(const std::string vertex_id){
    std::stack<char> Stack;
    for (char const &c: vertex_id){
        if (c == '['){
            Stack.push(c);
        }
        else if (c == ']'){
            if (Stack.empty())
                return false;
            else
                Stack.pop();
        }
    }
    return Stack.empty() ? true : false;
}

bool Graph::legalSemicolon(const std::string vertex_id){
    std::stack<char> Stack;
    for (char const &c: vertex_id){
        if (c == '['){
            Stack.push(c);
        }
        else if (c == ']'){
            if (Stack.empty())
                return false;
            else
                Stack.pop();
        }
        else if (c == ';'){
            if (Stack.empty())
                return false;
        }
    }
    return Stack.empty() ? true : false;
}

Graph& Graph::operator=(const Graph& other){
   if(this == &other){
        return *this;
    }
    this->Vertices=other.Vertices;
    return *this;
}

Graph operator+(const Graph& graph_1, const Graph& graph_2){
    Graph result = graph_1;
    for (auto it = graph_2.Vertices.begin(); it!=graph_2.Vertices.end(); ++it){
        if (!result.Vertices.count(it->first)){ //vertex does not exist yet - add it to result
            result.Vertices[it->first]=it->second;
        }
        //else - the vertex does exist. need to unite the sets of vertices connected to it:
        else{
            for (auto internal_it = it->second.begin(); internal_it != it->second.end(); internal_it++){ //iterator for the set
                (result.Vertices[it->first]).insert(*internal_it);
            }
        }
    }
    return result;
}

Graph operator^(const Graph& graph_1, const Graph& graph_2){
    Graph result;
    for (auto it = graph_1.Vertices.begin(); it!=graph_1.Vertices.end(); ++it){
        if (graph_2.Vertices.count(it->first)){ //vertex exists in graph 2 - add to result
        //if there are no edges for this vertex, copy only the vertex:
        std::set<std::string> Edges;
        result.Vertices[it->first]=Edges;
            for (auto internal_it = it->second.begin(); internal_it != it->second.end(); internal_it++){ //iterator for the set
                if ((graph_2.Vertices.at(it->first)).count(*internal_it))
                    (result.Vertices[it->first]).insert(*internal_it);
            }
        }
    }
    return result;
}

Graph operator-(const Graph& graph_1, const Graph& graph_2){
    Graph result;
    for (auto it = graph_1.Vertices.begin(); it!=graph_1.Vertices.end(); ++it){
        if (!graph_2.Vertices.count(it->first)){ //vertex does not exist in graph 2 - add to result
            std::set<std::string> Edges;
            result.Vertices[it->first]=Edges;
            for (auto internal_it = it->second.begin(); internal_it != it->second.end(); internal_it++){ //iterator for the set
                if (!graph_2.Vertices.count(*internal_it)){ 
                    (result.Vertices[it->first]).insert(*internal_it);
                }
            }
        }
    }
    return result;
}

Graph operator*(const Graph& graph_1, const Graph& graph_2){
    Graph result;
    //create new vertices: [v1;v2]
    for (auto it1 = graph_1.Vertices.begin(); it1!=graph_1.Vertices.end(); ++it1){
        for (auto it2 = graph_2.Vertices.begin(); it2!=graph_2.Vertices.end(); ++it2){
            std::string new_vertex_id = "["+it1->first+";"+it2->first+"]";
            std::set<std::string> Edges;
            result.Vertices[new_vertex_id]=Edges;
            //connect vertices
            for (auto internal_it1 = it1->second.begin(); internal_it1 != it1->second.end(); internal_it1++){
                for (auto internal_it2 = it2->second.begin(); internal_it2 != it2->second.end(); internal_it2++){
                    std::string edge_to = "["+*internal_it1+";"+*internal_it2+"]";
                    result.Vertices[new_vertex_id].insert(edge_to);
                }
            }
        }
    }
    return result;
}


Graph Graph::operator!() const{
    Graph result = *this;
    //clear edges
    for (auto it_r = result.Vertices.begin(); it_r!=result.Vertices.end(); ++it_r){
        (it_r->second).clear();
    }
    for (auto it_o = Vertices.begin(); it_o!=Vertices.end(); it_o++){
        for (auto it_r = result.Vertices.begin(); it_r!=result.Vertices.end(); it_r++){
                if (!(Vertices.at(it_o->first)).count(it_r->first) && it_o->first!=it_r->first)
                    (result.Vertices[it_o->first]).insert(it_r->first);
        }
    }
    return result;
}


std::ostream& operator<<(std::ostream& os, const Graph& graph)
{
    for (auto it = graph.Vertices.begin(); it!=graph.Vertices.end(); ++it){
        os << (*it).first << std::endl;
    }
    os << "$" << std::endl;
    for (auto it = graph.Vertices.begin(); it!=graph.Vertices.end(); ++it){
        for (auto internal_it = (*it).second.begin(); internal_it != (*it).second.end(); internal_it++){
            os << (*it).first << " " << *internal_it << std::endl;
        }
    }
    return os;
}

std::string Graph::readVertexIdFromFile(std::ifstream &infile, std::string file_name){
    unsigned int length = 0;
    infile.read((char*)&length, sizeof(unsigned int));
    if (length <= 0){
        throw InvalidReadFromFile(file_name);
    }
    if (infile.peek() == EOF){
        throw NotEnoughInput();
    }
    char* buffer = new char[length+1];
    infile.read(buffer, length);
    buffer[length]='\0';
    std::string vertex_id(buffer); 
    delete[] buffer;
    return vertex_id;
}

void Graph::readGraphFromFile(const std::string file_name){
    std::ifstream infile(file_name, std::ios_base::binary);
    if (!infile) {
        throw CannotOpenFile(file_name);
    }
    unsigned int num_vertices = 0, num_edges = 0;
    infile.read((char*)&num_vertices, sizeof(unsigned int));
    infile.read((char*)&num_edges, sizeof(unsigned int));
    if (num_vertices < 0 || num_edges < 0){
        throw InvalidReadFromFile(file_name);
    }
    for (unsigned int i = 0; i < num_vertices; i++){
        //read id length to int
        std::string vertex_id = readVertexIdFromFile(infile, file_name);
        addVertex(vertex_id);
    }
    for (unsigned int i = 0; i < num_edges; i++){        
        std::string source_vertex = readVertexIdFromFile(infile, file_name);
        std::string dest_vertex = readVertexIdFromFile(infile, file_name);
        addEdge(source_vertex, dest_vertex);
    }
    if (infile.peek() != EOF){
        throw NotEnoughInput();
    }        
}

void Graph::writeStringToFile(std::string str, std::ofstream &outfile){
    //convert to char*
    unsigned int n = str.length();
    const char *buffer = str.c_str();
    //write to file
    outfile.write((const char*)&n, sizeof(unsigned int));
    outfile.write(buffer, n);
}

void Graph::writeGraphToFile(const std::string file_name) const{
    std::ofstream outfile(file_name, std::ios_base::binary);
    if (!outfile) {
        throw CannotOpenFile(file_name);
    }
    // write int num of vertices and edges to start of file
    unsigned int numVertices = Vertices.size();
    unsigned int numEdges = getNumEdges();
    outfile.write((const char*)&numVertices, sizeof(unsigned int));
    outfile.write((const char*)&numEdges, sizeof(unsigned int));
    // write each vertex to file
    for (auto it = Vertices.begin(); it!=Vertices.end(); ++it){
        std::string vertex_id = (*it).first;
        writeStringToFile(vertex_id, outfile);
    }
    for (auto it = Vertices.begin(); it!= Vertices.end(); ++it){
        for (auto internal_it = (*it).second.begin(); internal_it != (*it).second.end(); internal_it++){
            std::string source_vertex = (*it).first;
            std::string dest_vertex = *internal_it;
            writeStringToFile(source_vertex, outfile);
            writeStringToFile(dest_vertex, outfile);
        }
    }    
}
