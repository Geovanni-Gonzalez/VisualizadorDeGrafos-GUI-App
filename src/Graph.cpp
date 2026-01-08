#include "Graph.h"
#include <iostream>

// Vertex Implementation
Vertex::Vertex(int id, std::string label, int x, int y) 
    : id(id), label(label), x(x), y(y) {}

// Edge Implementation
Edge::Edge(Vertex* src, Vertex* dst, int w) 
    : source(src), dest(dst), weight(w) {}

// Graph Implementation
Graph::Graph(bool directed, bool weighted) 
    : directed(directed), weighted(weighted) {}

Graph::~Graph() {
    clear();
}

Vertex* Graph::addVertex(int id, const std::string& label, int x, int y) {
    // Check if ID exists
    for (auto v : vertices) {
        if (v->id == id) return nullptr;
    }
    Vertex* newVertex = new Vertex(id, label, x, y);
    vertices.push_back(newVertex);
    return newVertex;
}

Vertex* Graph::getVertex(int id) {
    for (auto v : vertices) {
        if (v->id == id) return v;
    }
    return nullptr;
}

bool Graph::addEdge(int srcId, int destId, int weight) {
    Vertex* src = getVertex(srcId);
    Vertex* dest = getVertex(destId);

    if (!src || !dest) return false;

    // Check if edge already exists
    for (auto e : src->edges) {
        if (e->dest == dest) return false; 
    }

    Edge* newEdge = new Edge(src, dest, weight);
    src->edges.push_back(newEdge);

    if (!directed) {
        // For undirected, add reverse edge (usually with same weight)
        Edge* reverseEdge = new Edge(dest, src, weight);
        dest->edges.push_back(reverseEdge);
    }

    return true;
}

bool Graph::removeVertex(int id) {
    Vertex* vToRemove = getVertex(id);
    if (!vToRemove) return false;

    // Remove all edges pointing to this vertex from other vertices
    for (auto v : vertices) {
        // We need a robust way to remove while iterating or multiple passes.
        // Let's use a collection of edges to remove.
        LinkedList<Edge*> toDelete;
        for (auto e : v->edges) {
            if (e->dest == vToRemove) {
                toDelete.push_back(e);
            }
        }
        
        for (auto e : toDelete) {
            v->edges.remove(e);
            delete e;
        }
    }

    // Remove edges originating from this vertex
    for (auto e : vToRemove->edges) {
        delete e;
    }
    vToRemove->edges.clear();

    // Finally remove vertex from graph list
    vertices.remove(vToRemove);
    delete vToRemove;
    return true;
}

bool Graph::removeEdge(int srcId, int destId) {
    Vertex* src = getVertex(srcId);
    Vertex* dest = getVertex(destId);

    if (!src || !dest) return false;

    auto removeSingleEdge = [&](Vertex* from, Vertex* to) {
        for (auto e : from->edges) {
            if (e->dest == to) {
                from->edges.remove(e);
                delete e;
                return true;
            }
        }
        return false;
    };

    bool removed = removeSingleEdge(src, dest);
    if (!directed) {
        removeSingleEdge(dest, src);
    }
    
    return removed;
}

LinkedList<Vertex*>& Graph::getVertices() {
    return vertices;
}

void Graph::clear() {
    for (auto v : vertices) {
        for (auto e : v->edges) {
            delete e;
        }
        delete v;
    }
    vertices.clear();
}

bool Graph::isDirected() const { return directed; }
bool Graph::isWeighted() const { return weighted; }
