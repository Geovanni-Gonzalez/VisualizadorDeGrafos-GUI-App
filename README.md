# Visualizador de Grafos Interactive Studio 🚀

Una herramienta profesional y orientada al diseño para el modelado, edición y análisis de teoría de grafos, desarrollada íntegramente en C++ con Qt 5.

![Visualizador de Grafos](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Qt](https://img.shields.io/badge/Framework-Qt5-green.svg)
![License](https://img.shields.io/badge/Status-Finalizada-orange.svg)

## ✨ Características Principales

- **🎨 Interfaz de Usuario Moderna**: Diseño estilo *Glassmorphism* con soporte para modo oscuro y estética profesional.
- **🏗️ Construcción Interactiva**:
  - Agregado manual de nodos y aristas con etiquetas personalizadas.
  - Soporte para grafos **Dirigidos**, **No Dirigidos**, **Pesados** y **No Pesados**.
  - Sistema de **Arrastrar y Soltar** para reposicionar nodos.
- **🧠 Algoritmos Implementados**:
  - **Dijkstra**: Caminos más cortos desde un origen.
  - **Prim**: Generación de Árbol de Expansión Mínima (MST).
  - **Floyd-Warshall**: Caminos más cortos entre todos los pares.
  - **Detección de Ciclos**: Basado en DFS.
  - **Matriz de Caminos**: Análisis de conectividad.
  - **Centralidad**: Análisis de importancia de nodos.
- **⚙️ Herramientas de Productividad**:
  - **Sistema Deshacer/Rehacer (Undo/Redo)**: Control total sobre tus ediciones (`Ctrl+Z`, `Ctrl+Y`).
  - **Disposición Automática**: Algoritmo basado en fuerzas físicas para organizar el grafo automáticamente.
  - **Navegación Eficiente**: Zoom fluido con la rueda del ratón y paneo con clic derecho.
- **💾 Persistencia e Intercambio**:
  - Guardado y carga en formato nativo `.graph`.
  - Exportación de capturas en formato **PNG**.

## 🚀 Instalación y Compilación

### Requisitos previos

- Compilador de C++ (GCC/MinGW con soporte C++17).
- **Qt 5.x** (Widgets y Core).
- **CMake** (3.5 o superior).

### Pasos para compilar

1. Clona el repositorio:

   ```bash
   git clone https://github.com/Geovanni-Gonzalez/VisualizadorDeGrafos-GUI-App.git
   cd VisualizadorDeGrafos-GUI-App
   ```

2. Crea una carpeta de construcción y compila:

   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. Ejecuta la aplicación:

   ```bash
   ./VisualizadorDeGrafos
   ```

## 🎮 Guía de Uso Rápido

1. **Nuevo Grafo**: Define las propiedades (dirigido/pesado) en el arranque.
2. **Modos de Edición**: Usa la barra de herramientas superior para cambiar entre:
   - ➕ **Nodo**: Haz clic en el lienzo para crear uno.
   - 🔗 **Arista**: Arrastra desde un nodo de origen a uno de destino.
   - 🖐️ **Mover**: Reposiciona tus nodos.
   - ❌ **Eliminar**: Haz clic en un elemento para quitarlo.
3. **Navegación**:
   - 🖱️ **Zoom**: Rueda del ratón.
   - ↔️ **Paneo**: Clic derecho y arrastrar.
4. **Análisis**: Selecciona cualquier algoritmo del menú para visualizar los resultados matemáticos en el panel lateral.

## 🛠️ Tecnologías Utilizadas

- **Lenguaje**: C++17.
- **Framework GUI**: Qt 5.
- **Estructuras de Datos**: Implementación manual de `LinkedList` para el motor del grafo (sin dependencias de STL para lógica central).
- **Build System**: CMake.

---
Desarrollado con ❤️ para el modelado de estructuras de datos y teoría de grafos.
