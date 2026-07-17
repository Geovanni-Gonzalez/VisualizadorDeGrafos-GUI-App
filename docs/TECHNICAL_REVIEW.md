# TECHNICAL_REVIEW — VisualizadorDeGrafos-GUI-App

Fecha de revisión: 2026-07-16
Método: análisis estático, enunciado (`docs/Enunciado_VisualizadorDeGrafos_PY04.md`), CMake, CI y git. Sin ejecución en esta pasada (Qt5 no instalado en el sandbox); CI compila con Qt5 real en Ubuntu.

## 1. Comprensión del proyecto

Visualizador y editor gráfico de grafos en **C++17 + Qt5** (~2,180 LOC): construcción/edición de grafos dirigidos/no dirigidos y pesados/no pesados, persistencia a archivo, y análisis con algoritmos clásicos implementados a mano sobre estructuras propias (template `LinkedList` doblemente enlazada).

## 2. Cumplimiento del enunciado

| Requisito | Estado | Evidencia |
|---|---|---|
| Construcción/edición gráfica (ventanas, no consola) | 🟦 | `MainWindow.cpp` (577 LOC), `GraphView.cpp` (440), `VisualItems.h` |
| Grafo dirigido/no dirigido, pesado/no pesado | ✅ estático | `Graph(true, true)` flags; test `VerifyLogic.cpp` |
| Guardar/cargar desde archivo | 🟦 | `FileController.{h,cpp}` |
| Matriz de caminos | ✅ estático | `GraphAlgorithms::getPathMatrix` |
| Camino más corto desde un vértice | ✅ estático | `getShortestPathDijkstra` |
| Todos los pares de caminos más cortos | ✅ estático | `getAllPairsShortestPaths` (matriz + idMap) |
| Árbol de expansión mínima | ✅ estático | `getMSTPrim` (comentario honesto: "Basic Prim's O(V*E)") |
| Extra no exigido: centralidad de cercanía | ✅ estático | `getClosenessCentrality` |

## 3. Calidad y pruebas

- ✅ `tests/VerifyLogic.cpp` (84 LOC, ~19 asserts): grafo básico, detección de ciclos, algoritmos — compilado como target separado sin GUI (buena separación).
- ✅ CI compila ambos targets y ejecuta `VerifyLogic`.
- `LinkedList` con constructor de copia explícito para evitar punteros colgantes — conciencia de rule-of-three; gestión manual de matrices con `freeMatrix` pareados.

## 4. Fortalezas

1. Algoritmos de grafos implementados a mano (Dijkstra, all-pairs, Warshall, Prim, closeness) sobre estructuras propias — sin STL containers para el core, como exigen estos cursos.
2. Separación modelo/algoritmos/GUI real: los tests linkan solo `Graph`+`GraphAlgorithms`+`FileController` con `Qt5::Core`.
3. Complejidades reconocidas en comentarios (señal de honestidad técnica).

## 5. Debilidades y riesgos

| Hallazgo | Severidad | Nota |
|---|---|---|
| Cobertura limitada de `VerifyLogic` | Media | Faltan casos de grafo desconexo, pesos inválidos e ida/vuelta de archivo |
| Memoria manual (`bool**`, `int**`, `new/delete`) sin RAII | Media | Correcto si los `freeMatrix` se llaman; smart pointers serían más defendibles |
| Qt5 (EOL comercial) en vez de Qt6 | Baja | Migración simple |
| Sin capturas de la GUI en el README más allá de una imagen | Baja | La GUI es el punto del proyecto |

## 6. Evaluación profesional

- Nivel demostrado: **Mid en C++ / estructuras de datos**. Implementación algorítmica correcta y arquitectura por capas; le falta RAII/moderno y CI con ejecución para más.
- Rol en el portafolio: la pieza de **algoritmos de grafos + C++ GUI**; complementa a `XMLDocument-TreeBuilder` y `LibreriaString` en el bloque C++.

## 7. Recomendaciones

Ver `IMPROVEMENT_ROADMAP.md`. P0 de CI aplicado; siguiente foco: ampliar `VerifyLogic`.
