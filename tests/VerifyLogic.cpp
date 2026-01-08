#include <iostream>
#include <cassert>
#include "Graph.h"
#include "GraphAlgorithms.h"

void testBasicGraph() {
    std::cout << "Prueba: Creación básica de grafo... ";
    Graph g(true, true);
    g.addVertex(1, "A", 0, 0);
    g.addVertex(2, "B", 10, 10);
    g.addEdge(1, 2, 5);
    
    assert(g.getVertices().size() == 2);
    Vertex* v1 = g.getVertex(1);
    assert(v1 != nullptr);
    assert(v1->edges.size() == 1);
    assert(v1->edges.front()->weight == 5);
    std::cout << "PASÓ" << std::endl;
}

void testCycles() {
    std::cout << "Prueba: Detección de ciclos... ";
    
    // Directed cycle
    Graph gd(true, false);
    gd.addVertex(1, "1", 0,0);
    gd.addVertex(2, "2", 0,0);
    gd.addVertex(3, "3", 0,0);
    gd.addEdge(1, 2, 0);
    gd.addEdge(2, 3, 0);
    gd.addEdge(3, 1, 0);
    assert(GraphAlgorithms::hasCycles(&gd) == true);
    
    // Directed acyclic
    Graph gda(true, false);
    gda.addVertex(1, "1", 0,0);
    gda.addVertex(2, "2", 0,0);
    gda.addEdge(1, 2, 0);
    assert(GraphAlgorithms::hasCycles(&gda) == false);

    // Undirected cycle
    Graph gu(false, false);
    gu.addVertex(1, "1", 0,0);
    gu.addVertex(2, "2", 0,0);
    gu.addVertex(3, "3", 0,0);
    gu.addEdge(1, 2, 0);
    gu.addEdge(2, 3, 0);
    gu.addEdge(3, 1, 0);
    assert(GraphAlgorithms::hasCycles(&gu) == true);

    std::cout << "PASÓ" << std::endl;
}

void testDijkstra() {
    std::cout << "Prueba: Dijkstra... ";
    Graph g(true, true);
    g.addVertex(1, "A", 0, 0);
    g.addVertex(2, "B", 0, 0);
    g.addVertex(3, "C", 0, 0);
    g.addEdge(1, 2, 10);
    g.addEdge(2, 3, 5);
    g.addEdge(1, 3, 20);
    
    LinkedList<Edge*> path = GraphAlgorithms::getShortestPathDijkstra(&g, 1, 3);
    assert(path.size() == 2); // 1->2, 2->3 is length 15
    assert(path.front()->source->id == 1);
    assert(path.front()->dest->id == 2);
    assert(path.back()->dest->id == 3);
    
    std::cout << "PASÓ" << std::endl;
}

int main() {
    try {
        testBasicGraph();
        testCycles();
        testDijkstra();
        std::cout << "\n¡Todas las pruebas de lógica pasaron exitosamente!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "\nFALLO EN PRUEBA: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
