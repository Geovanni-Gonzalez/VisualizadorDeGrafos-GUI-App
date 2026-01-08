#ifndef GRAPH_H
#define GRAPH_H

#include "LinkedList.h"
#include <string>

struct Edge; 

struct Vertex {
    int id;
    std::string label;
    int x, y; // GUI coordinates
    LinkedList<Edge*> edges; // Adjacency list

    Vertex(int id, std::string label, int x, int y);
};

struct Edge {
    Vertex* source;
    Vertex* dest;
    int weight;
    bool isDirected;

    Edge(Vertex* src, Vertex* dst, int w);
};

class Graph {
public:
    Graph(bool directed = false, bool weighted = false);
    ~Graph();

    Vertex* addVertex(int id, const std::string& label, int x, int y);
    bool addEdge(int srcId, int destId, int weight = 1);
    bool removeVertex(int id);
    bool removeEdge(int srcId, int destId);
    Vertex* getVertex(int id);
    
    LinkedList<Vertex*>& getVertices();
    void clear();

    bool isDirected() const;
    bool isWeighted() const;

private:
    LinkedList<Vertex*> vertices;
    bool directed;
    bool weighted;
};

#endif // GRAPH_H
