# IMPROVEMENT_ROADMAP — VisualizadorDeGrafos-GUI-App

Backlog priorizado. Impacto/Esfuerzo: Alto/Medio/Bajo.

## Quick Wins

| # | Mejora | Impacto | Esfuerzo | Prioridad |
|---|---|---|---|---|
| 1 | ✅ Aplicado: CI ejecuta `./build/VerifyLogic` después del build CMake | Alto | Bajo | P0 |
| 2 | GitHub Topics: `cpp`, `qt`, `graph-algorithms`, `dijkstra`, `data-structures`, `cmake` + descripción | Medio | Bajo | P1 |
| 3 | 2-3 capturas de la GUI en acción (grafo cargado, Dijkstra resaltado, MST) — la GUI es el diferenciador y solo hay 1 imagen | Medio | Bajo | P1 |

## Mejoras técnicas

| # | Mejora | Impacto | Esfuerzo | Prioridad |
|---|---|---|---|---|
| 4 | RAII para matrices: `std::vector<std::vector<...>>` o wrapper con destructor en vez de `bool**`/`int**` + `freeMatrix` | Medio | Medio | P2 |
| 5 | Prim con priority queue (O(E log V)) — el comentario ya reconoce el O(V·E) actual | Medio | Medio | P2 |
| 6 | Más casos en `VerifyLogic` (grafos desconexos, pesos negativos rechazados, ida/vuelta de archivo) | Medio | Bajo | P2 |

## Mejoras arquitectónicas

| # | Mejora | Impacto | Esfuerzo | Prioridad |
|---|---|---|---|---|
| 7 | Migrar a Qt6 (cambios menores en CMake) | Bajo | Bajo | P3 |
| 8 | Patrón Command completo para undo/redo en edición (existe `include/Command.h` — completarlo o retirarlo) | Medio | Alto | P3 |

## Mejoras de GitHub

Ya presentes: badge CI, LICENSE, README caso-de-estudio, enunciado en docs. Faltan: Topics (item 2), capturas (item 3).
