#ifndef GRAPHALGORITHMS_H
#define GRAPHALGORITHMS_H

#include "Graph.h"
#include <limits>

class GraphAlgorithms {
public:
    static bool** getPathMatrix(Graph* graph, int& size);
    static LinkedList<Edge*> getShortestPathDijkstra(Graph* graph, int startId, int endId);
    static int** getAllPairsShortestPaths(Graph* graph, int& size, int*& idMap);
    static LinkedList<Edge*> getMSTPrim(Graph* graph);
    static bool hasCycles(Graph* graph);
    static double* getClosenessCentrality(Graph* graph, int*& idMap, int& size);

    // Helpers
    static void freeMatrix(bool** matrix, int size);
    static void freeMatrix(int** matrix, int size);
};

#endif // GRAPHALGORITHMS_H
