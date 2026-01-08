#include "GraphAlgorithms.h"
#include <iostream>

const int INF = 1e9;

// Helper to map IDs to 0..N-1 indices
void mapIdsToIndices(Graph* graph, int*& idMap, int& size) {
    size = graph->getVertices().size();
    idMap = new int[size];
    int i = 0;
    for (auto v : graph->getVertices()) {
        idMap[i++] = v->id;
    }
}

int getIndex(int id, int* idMap, int size) {
    for (int i = 0; i < size; ++i) {
        if (idMap[i] == id) return i;
    }
    return -1;
}

bool** GraphAlgorithms::getPathMatrix(Graph* graph, int& size) {
    int* idMap;
    mapIdsToIndices(graph, idMap, size);

    // Allocate matrix
    bool** matrix = new bool*[size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new bool[size];
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = false;
        }
    }

    // Initialize with direct edges
    int i = 0;
    for (auto v : graph->getVertices()) {
        for (auto e : v->edges) {
            int j = getIndex(e->dest->id, idMap, size);
            if (j != -1) matrix[i][j] = true;
        }
        i++;
    }

    // Warshall's Algorithm for transitive closure
    for (int k = 0; k < size; ++k) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                matrix[i][j] = matrix[i][j] || (matrix[i][k] && matrix[k][j]);
            }
        }
    }

    delete[] idMap;
    return matrix;
}

LinkedList<Edge*> GraphAlgorithms::getShortestPathDijkstra(Graph* graph, int startId, int endId) {
    LinkedList<Edge*> path;
    int size = graph->getVertices().size();
    if (size == 0) return path;

    // We need a way to map Vertex* to dist/pred
    // Since we can't use map/vector, we can add temp properties to Vertex? 
    // Or use parallel arrays if we map to indices.
    
    int* idMap;
    int realSize;
    mapIdsToIndices(graph, idMap, realSize);
    
    int startIdx = getIndex(startId, idMap, realSize);
    int endIdx = getIndex(endId, idMap, realSize);

    if (startIdx == -1 || endIdx == -1) {
        delete[] idMap;
        return path;
    }

    int* dist = new int[realSize];
    int* pred = new int[realSize]; // Predecessor index
    bool* visited = new bool[realSize];

    for (int i = 0; i < realSize; ++i) {
        dist[i] = INF;
        pred[i] = -1;
        visited[i] = false;
    }

    dist[startIdx] = 0;

    for (int i = 0; i < realSize; ++i) {
        // Find min dist node not visited
        int u = -1;
        int minVal = INF;
        for (int j = 0; j < realSize; ++j) {
            if (!visited[j] && dist[j] < minVal) {
                minVal = dist[j];
                u = j;
            }
        }

        if (u == -1 || dist[u] == INF) break;
        visited[u] = true;

        if (u == endIdx) break; // Reached target

        // Relax edges
        Vertex* uVertex = graph->getVertex(idMap[u]);
        for (auto e : uVertex->edges) {
            int v = getIndex(e->dest->id, idMap, realSize);
            if (v != -1 && !visited[v]) {
                if (dist[u] + e->weight < dist[v]) {
                    dist[v] = dist[u] + e->weight;
                    pred[v] = u;
                }
            }
        }
    }

    // Reconstruct path
    if (dist[endIdx] != INF) {
        int curr = endIdx;
        while (curr != -1 && curr != startIdx) {
            int prev = pred[curr];
            if (prev != -1) {
                Vertex* uVertex = graph->getVertex(idMap[prev]);
                Vertex* vVertex = graph->getVertex(idMap[curr]);
                // Find edge
                for (auto e : uVertex->edges) {
                    if (e->dest == vVertex) {
                        // Ideally we should pick the specific edge with min weight if multi-graph, 
                        // but here we just take the first one or check weight? 
                        // Dijkstra guarantees we used *an* edge. 
                        // Actually we should store the Edge* in pred but we stored index.
                        // Let's just find the edge that matches the weight logic or just the connection.
                        if (dist[prev] + e->weight == dist[curr]) {
                             path.push_back(e);
                             break;
                        }
                    }
                }
            }
            curr = prev;
        }
    }
    
    // Path is currently reversed (End -> Start). We might want to reverse it or let the GUI handle it.
    // LinkedList doesn't have reverse easily.
    // Let's create a new list in correct order.
    // Wait, simple usage of LinkedList means simple push_back. 
    // To reverse, we can just push to a new list or iterate backwards if it was distinct nodes.
    // Since we need to draw it, order matters for animation/drawing direction? 
    // For now, let's keep it reversed or fix it. A simple fix is to use a stack or just creating a new list.
    
    LinkedList<Edge*> correctOrderPath;
    // ... manual reverse ... but for now, let's just return it. 
    // Actually, let's fix it.
    // LinkedList doesn't have random access.
    // Let's re-build it? No.
    // We can rely on the fact it's doubly linked. Use tail and iterate back?
    // Oh, my LinkedList has `tail`. I can iterate reverse if I added `rbegin`?
    // I didn't add iterators for reverse.
    
    // Quick fix: Use a temporary array of Edge* then push_back to new list.
    int pathCount = path.size();
    if (pathCount > 0) {
        Edge** temp = new Edge*[pathCount];
        int k = 0;
        for (auto e : path) {
            temp[k++] = e;
        }
        for (int j = pathCount - 1; j >= 0; --j) {
            correctOrderPath.push_back(temp[j]);
        }
        delete[] temp;
    }
    
    delete[] dist;
    delete[] pred;
    delete[] visited;
    delete[] idMap;
    return correctOrderPath;
}

int** GraphAlgorithms::getAllPairsShortestPaths(Graph* graph, int& size, int*& idMap) {
    mapIdsToIndices(graph, idMap, size);

    int** dist = new int*[size];
    for (int i = 0; i < size; ++i) {
        dist[i] = new int[size];
        for (int j = 0; j < size; ++j) {
            if (i == j) dist[i][j] = 0;
            else dist[i][j] = INF;
        }
    }

    int i = 0;
    for (auto v : graph->getVertices()) {
        for (auto e : v->edges) {
            int j = getIndex(e->dest->id, idMap, size);
            if (j != -1) {
                // Handle directed/undirected inside graph, here we just read edges
                if (e->weight < dist[i][j]) {
                    dist[i][j] = e->weight;
                }
            }
        }
        i++;
    }

    // Floyd-Warshall
    for (int k = 0; k < size; ++k) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    return dist;
}

LinkedList<Edge*> GraphAlgorithms::getMSTPrim(Graph* graph) {
    LinkedList<Edge*> mst;
    int size;
    int* idMap;
    mapIdsToIndices(graph, idMap, size);
    
    if (size == 0) {
         delete[] idMap; 
         return mst;
    }

    bool* visited = new bool[size];
    for (int i = 0; i < size; ++i) visited[i] = false;

    // Start from first node
    visited[0] = true;
    int numEdges = 0;

    // Basic Prim's O(V*E)
    // Repeat V-1 times
    while (numEdges < size - 1) {
        int minWeight = INF;
        Vertex* startNode = nullptr;
        Edge* minEdge = nullptr;

        // Iterate all visited nodes
        int uIdx = 0;
        for (auto u : graph->getVertices()) {
            if (visited[uIdx]) { // Only consider edges from visited set
                 for (auto e : u->edges) {
                     int vIdx = getIndex(e->dest->id, idMap, size);
                     if (vIdx != -1 && !visited[vIdx]) { // To unvisited
                         if (e->weight < minWeight) {
                             minWeight = e->weight;
                             minEdge = e; 
                         }
                     }
                 }
            }
            uIdx++;
        }

        if (minEdge) {
            mst.push_back(minEdge);
            // Mark dest as visited
            int targetIdx = getIndex(minEdge->dest->id, idMap, size);
            visited[targetIdx] = true;
            numEdges++;
        } else {
            // Disconnected graph or done
            break;
        }
    }

    delete[] visited;
    delete[] idMap;
    return mst;
}

void GraphAlgorithms::freeMatrix(bool** matrix, int size) {
    for (int i = 0; i < size; ++i) delete[] matrix[i];
    delete[] matrix;
}

void GraphAlgorithms::freeMatrix(int** matrix, int size) {
    for (int i = 0; i < size; ++i) delete[] matrix[i];
    delete[] matrix;
}
bool hasCyclesRecursive(Graph* graph, int u, bool* visited, bool* recStack, int* idMap, int size) {
    visited[u] = true;
    recStack[u] = true;

    Vertex* v = graph->getVertex(idMap[u]);
    for (auto e : v->edges) {
        int vIdx = getIndex(e->dest->id, idMap, size);
        if (vIdx != -1) {
            if (!visited[vIdx] && hasCyclesRecursive(graph, vIdx, visited, recStack, idMap, size))
                return true;
            else if (recStack[vIdx])
                return true;
        }
    }

    recStack[u] = false;
    return false;
}

bool hasCyclesUndirected(Graph* graph, int u, bool* visited, int parent, int* idMap, int size) {
    visited[u] = true;

    Vertex* vText = graph->getVertex(idMap[u]);
    for (auto e : vText->edges) {
        int vIdx = getIndex(e->dest->id, idMap, size);
        if (vIdx != -1) {
            if (!visited[vIdx]) {
                if (hasCyclesUndirected(graph, vIdx, visited, u, idMap, size))
                    return true;
            } else if (vIdx != parent) {
                return true;
            }
        }
    }
    return false;
}

bool GraphAlgorithms::hasCycles(Graph* graph) {
    int size;
    int* idMap;
    mapIdsToIndices(graph, idMap, size);
    if (size == 0) { delete[] idMap; return false; }

    bool* visited = new bool[size];
    for (int i = 0; i < size; ++i) visited[i] = false;

    bool result = false;
    if (graph->isDirected()) {
        bool* recStack = new bool[size];
        for (int i = 0; i < size; ++i) recStack[i] = false;
        
        for (int i = 0; i < size; ++i) {
            if (hasCyclesRecursive(graph, i, visited, recStack, idMap, size)) {
                result = true;
                break;
            }
        }
        delete[] recStack;
    } else {
        for (int i = 0; i < size; ++i) {
            if (!visited[i]) {
                if (hasCyclesUndirected(graph, i, visited, -1, idMap, size)) {
                    result = true;
                    break;
                }
            }
        }
    }

    delete[] visited;
    delete[] idMap;
    return result;
}
double* GraphAlgorithms::getClosenessCentrality(Graph* graph, int*& idMap, int& size) {
    int** dists = getAllPairsShortestPaths(graph, size, idMap);
    if (!dists) return nullptr;

    double* closeness = new double[size];
    for (int i = 0; i < size; ++i) {
        double sumDist = 0;
        int reachable = 0;
        for (int j = 0; j < size; ++j) {
            if (i != j && dists[i][j] < INF) {
                sumDist += dists[i][j];
                reachable++;
            }
        }
        if (sumDist > 0) {
            closeness[i] = (double)reachable / sumDist;
        } else {
            closeness[i] = 0;
        }
    }

    freeMatrix(dists, size);
    return closeness;
}
