# CV_EVIDENCE — VisualizadorDeGrafos-GUI-App

Verifiable, interview-defensible material. All claims map to files in this repository.

## Resume bullets (pick & adapt)

- Developed a graph visualization and editing desktop app in C++17 + Qt5: interactive canvas for building directed/undirected, weighted/unweighted graphs with file save/load.
- Implemented classic graph algorithms from scratch over custom data structures (hand-rolled doubly-linked template list): Dijkstra shortest path, all-pairs shortest paths, reachability (path) matrix, Prim's MST, and closeness centrality.
- Separated model/algorithms from the GUI so the logic compiles into a standalone assertion-based test binary (`VerifyLogic`) without widget dependencies; CMake build with GitHub Actions CI on Linux/Qt5.

## Skills matrix

| Skill | Evidence | Depth | Confidence |
|---|---|---|---|
| Graph algorithms (Dijkstra, all-pairs, Warshall-style path matrix, Prim, centrality) | `src/GraphAlgorithms.cpp` (392 LOC) | Medium-Deep | High |
| C++ templates & manual memory management | `include/LinkedList.h` (copy ctor for deep copies), `freeMatrix` pairs | Medium | High |
| Qt Widgets / Graphics View | `MainWindow.cpp`, `GraphView.cpp`, `VisualItems.h` | Medium | High |
| CMake (multi-target, AUTOMOC) | `CMakeLists.txt` | Medium | High |
| Testable architecture (GUI-free logic target) | `tests/VerifyLogic.cpp` + CMake test target | Medium | High |

## What this project proves

- First appearance of: graph algorithms implemented by hand, Qt GUI, CMake, C++ templates with ownership awareness.
- Reinforces: C++ (with LibreriaString, XMLDocument-TreeBuilder), CI, layered design.
- Portfolio role: the algorithms + native GUI evidence piece.

## ATS keywords

C++, C++17, Qt, Qt5, CMake, graph algorithms, Dijkstra, minimum spanning tree, Prim, shortest path, data structures, linked list, templates, memory management, GUI development, desktop applications, GitHub Actions.
