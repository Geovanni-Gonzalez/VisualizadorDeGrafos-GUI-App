#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include "Graph.h"
#include <string>

class FileController {
public:
    static bool saveGraph(Graph* graph, const std::string& filename);
    static Graph* loadGraph(const std::string& filename);
};

#endif // FILECONTROLLER_H
