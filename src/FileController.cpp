#include "FileController.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool FileController::saveGraph(Graph* graph, const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) return false;

    outfile << "TYPE:" << (graph->isDirected() ? "DIRECTED" : "UNDIRECTED") << "\n";
    outfile << "WEIGHTED:" << (graph->isWeighted() ? "TRUE" : "FALSE") << "\n";
    
    outfile << "NODES\n";
    for (auto v : graph->getVertices()) {
        outfile << v->id << "," << v->label << "," << v->x << "," << v->y << "\n";
    }

    outfile << "EDGES\n";
    // Avoid duplicates for undirected? 
    // Graph stores double edges for undirected. 
    // If we save all, when loading undirected we might duplicate again if addEdge adds both.
    // Logic: If undirected, only save if src < dest?
    // Or just save all and handle duplicates on load. 
    // Our addEdge checks for existence. So saving all is safe but redundant.
    
    for (auto v : graph->getVertices()) {
        for (auto e : v->edges) {
            outfile << e->source->id << "," << e->dest->id << "," << e->weight << "\n";
        }
    }

    outfile.close();
    return true;
}

Graph* FileController::loadGraph(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) return nullptr;

    std::string line;
    bool directed = false;
    bool weighted = false;
    
    // Header
    if (std::getline(infile, line)) {
        if (line.find("DIRECTED") != std::string::npos) directed = true;
    }
    if (std::getline(infile, line)) {
        if (line.find("WEIGHTED:TRUE") != std::string::npos) weighted = true;
    }

    Graph* graph = new Graph(directed, weighted);
    
    std::string section = "";
    while (std::getline(infile, line)) {
        if (line == "NODES") { section = "NODES"; continue; }
        if (line == "EDGES") { section = "EDGES"; continue; }
        
        if (section == "NODES") {
             std::stringstream ss(line);
             std::string segment;
             std::string parts[4];
             int i = 0;
             while(std::getline(ss, segment, ',') && i < 4) {
                parts[i++] = segment;
             }
             if(i >= 4) {
                 int id = std::stoi(parts[0]);
                 std::string label = parts[1];
                 int x = std::stoi(parts[2]);
                 int y = std::stoi(parts[3]);
                 graph->addVertex(id, label, x, y);
             }
        } else if (section == "EDGES") {
             std::stringstream ss(line);
             std::string segment;
             std::string parts[3];
             int i = 0;
             while(std::getline(ss, segment, ',') && i < 3) {
                parts[i++] = segment;
             }
             if(i >= 3) {
                 int src = std::stoi(parts[0]);
                 int dest = std::stoi(parts[1]);
                 int w = std::stoi(parts[2]);
                 graph->addEdge(src, dest, w);
             }
        }
    }

    infile.close();
    return graph;
}
