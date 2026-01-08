#include "MainWindow.h"
#include "GraphAlgorithms.h"
#include "FileController.h"
#include "Command.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QTextEdit>
#include <QStatusBar>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <cmath>

class MoveNodeCommand : public Command {
    GraphView* view;
    int nodeId;
    QPointF oldPos;
    QPointF newPos;
public:
    MoveNodeCommand(GraphView* v, int id, QPointF oldP, QPointF newP) 
        : view(v), nodeId(id), oldPos(oldP), newPos(newP) {}
    void execute() override { 
        Vertex* v = view->getGraph()->getVertex(nodeId);
        if(v) { v->x = newPos.x(); v->y = newPos.y(); view->setGraph(view->getGraph()); }
    }
    void undo() override {
        Vertex* v = view->getGraph()->getVertex(nodeId);
        if(v) { v->x = oldPos.x(); v->y = oldPos.y(); view->setGraph(view->getGraph()); }
    }
};

class AddNodeCommand : public Command {
    GraphView* view;
    int nodeId;
    std::string label;
    double x, y;
public:
    AddNodeCommand(GraphView* v, int id, std::string l, double xPos, double yPos)
        : view(v), nodeId(id), label(l), x(xPos), y(yPos) {}
    void execute() override {
        view->getGraph()->addVertex(nodeId, label, x, y);
        view->setGraph(view->getGraph());
    }
    void undo() override {
        // Need to add removeVertex to Graph
        view->getGraph()->removeVertex(nodeId);
        view->setGraph(view->getGraph());
    }
};

class AddEdgeCommand : public Command {
    GraphView* view;
    int srcId, destId, weight;
public:
    AddEdgeCommand(GraphView* v, int s, int d, int w)
        : view(v), srcId(s), destId(d), weight(w) {}
    void execute() override {
        view->getGraph()->addEdge(srcId, destId, weight);
        view->setGraph(view->getGraph());
    }
    void undo() override {
        view->getGraph()->removeEdge(srcId, destId);
        view->setGraph(view->getGraph());
    }
};

class DeleteNodeCommand : public Command {
    GraphView* view;
    int nodeId;
    std::string label;
    double x, y;
    struct EdgeInfo { int src, dst, weight; };
    LinkedList<EdgeInfo> deletedEdges;
public:
    DeleteNodeCommand(GraphView* v, int id) : view(v), nodeId(id) {
        Vertex* vert = view->getGraph()->getVertex(id);
        if(vert) {
            label = vert->label;
            x = vert->x;
            y = vert->y;
            // Backup edges
            for(auto vertex : view->getGraph()->getVertices()) {
                for(auto e : vertex->edges) {
                    if(e->source->id == nodeId || e->dest->id == nodeId) {
                        deletedEdges.push_back({e->source->id, e->dest->id, e->weight});
                    }
                }
            }
        }
    }
    void execute() override {
        view->getGraph()->removeVertex(nodeId);
        view->setGraph(view->getGraph());
    }
    void undo() override {
        view->getGraph()->addVertex(nodeId, label, x, y);
        for(auto& ei : deletedEdges) {
            view->getGraph()->addEdge(ei.src, ei.dst, ei.weight);
        }
        view->setGraph(view->getGraph());
    }
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Visualizador De Grafos");
    resize(1024, 768);

    graphView = new GraphView(this);
    
    // UI Layout: Central area for Graph, Right sidebar for info
    QWidget* mainWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(mainWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10); // Added padding for "floating" effect
    mainLayout->setSpacing(10);
    
    mainLayout->addWidget(graphView, 1);
    
    // Sidebar
    QWidget* sidebar = new QWidget(this);
    sidebar->setFixedWidth(250);
    sidebar->setObjectName("sidebar");
    QVBoxLayout* sideLayout = new QVBoxLayout(sidebar);
    
    QLabel* titleLabel = new QLabel("Inspector de Grafos", sidebar);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; margin-bottom: 10px;");
    sideLayout->addWidget(titleLabel);
    
    // Stats
    statsLabel = new QLabel("Vértices: 0\nAristas: 0", sidebar);
    sideLayout->addWidget(statsLabel);
    
    sideLayout->addStretch();
    mainLayout->addWidget(sidebar);
    
    setCentralWidget(mainWidget); // Changed from setCentralWidget(centralWidget); to mainWidget

    connect(graphView, &GraphView::nodeMovedCommand, this, &MainWindow::handleNodeMovedCommand, Qt::QueuedConnection);
    connect(graphView, &GraphView::requestAddNode, this, &MainWindow::handleRequestAddNode, Qt::QueuedConnection);
    connect(graphView, &GraphView::requestAddEdge, this, &MainWindow::handleRequestAddEdge, Qt::QueuedConnection);
    connect(graphView, &GraphView::requestDeleteVertex, this, &MainWindow::handleRequestDeleteVertex, Qt::QueuedConnection);

    createActions();
    createToolBar();
    
    statusLabel = new QLabel("Listo");
    statusBar()->addWidget(statusLabel);
    
    connect(graphView, &GraphView::graphChanged, this, &MainWindow::updateStats);
    updateStats();
    
    applyStyle();
}

void MainWindow::updateStats() {
    int vCount = graphView->getGraph()->getVertices().size();
    int eCount = 0;
    for (auto v : graphView->getGraph()->getVertices()) {
        eCount += v->edges.size();
    }
    
    QString stats = QString("Vértices: %1\nAristas: %2").arg(vCount).arg(eCount);
    statsLabel->setText(stats);
}

void MainWindow::applyStyle() {
    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #1a1c1e;
        }
        QWidget#sidebar {
            background-color: rgba(45, 48, 52, 180);
            border: 1px solid rgba(255, 255, 255, 20);
            border-radius: 12px;
            color: #e2e2e2;
        }
        QToolBar {
            background-color: rgba(30, 32, 35, 200);
            border: 1px solid rgba(255, 255, 255, 10);
            border-radius: 10px;
            margin: 5px;
            padding: 4px;
        }
        QToolButton {
            color: #d3d3d3;
            background-color: transparent;
            border: 1px solid transparent;
            border-radius: 8px;
            padding: 8px;
            font-weight: bold;
            font-family: 'Segoe UI', sans-serif;
        }
        QToolButton:hover {
            background-color: rgba(255, 255, 255, 15);
            border: 1px solid rgba(255, 255, 255, 30);
        }
        QToolButton:checked {
            background-color: rgba(74, 144, 226, 40);
            border: 1px solid #4a90e2;
            color: #4a90e2;
        }
        QLabel {
            color: #b0b3b8;
            font-family: 'Segoe UI', sans-serif;
        }
        QStatusBar {
            background-color: transparent;
            color: #8e8e8e;
            border-top: 1px solid rgba(255, 255, 255, 5);
        }
        QInputDialog, QMessageBox, QDialog {
            background-color: #1a1c1e;
            color: #e2e2e2;
            border: 1px solid #333;
        }
        QPushButton {
            background-color: #3a3f44;
            color: white;
            border-radius: 6px;
            padding: 6px 15px;
            border: none;
        }
        QPushButton:hover {
            background-color: #4a90e2;
        }
    )");
}

MainWindow::~MainWindow() {
}

void MainWindow::createActions() {
    actionNew = new QAction("New", this);
    actionNew->setShortcut(QKeySequence::New);
    connect(actionNew, &QAction::triggered, this, &MainWindow::newGraph);
    
    actionLoad = new QAction("Load", this);
    actionLoad->setShortcut(QKeySequence::Open);
    connect(actionLoad, &QAction::triggered, this, &MainWindow::loadGraph);
    
    actionSave = new QAction("Guardar", this);
    actionSave->setShortcut(QKeySequence::Save);
    connect(actionSave, &QAction::triggered, this, &MainWindow::saveGraph);
    
    actionExportPNG = new QAction("Exportar como Imagen", this);
    connect(actionExportPNG, &QAction::triggered, this, &MainWindow::exportPNG);

    QAction* actionExportSVG = new QAction("Exportar como SVG (Vectorial)", this);
    connect(actionExportSVG, &QAction::triggered, this, &MainWindow::exportSVG);
    

    actionAddNode = new QAction("Add Node", this);
    actionAddNode->setCheckable(true);
    connect(actionAddNode, &QAction::triggered, this, &MainWindow::setModeAddNode);

    actionAddEdge = new QAction("Add Edge", this);
    actionAddEdge->setCheckable(true);
    connect(actionAddEdge, &QAction::triggered, this, &MainWindow::setModeAddEdge);

    actionMove = new QAction("Move/Select", this);
    actionMove->setCheckable(true);
    connect(actionMove, &QAction::triggered, this, &MainWindow::setModeMove);
    
    actionDelete = new QAction("Eliminar", this);
    actionDelete->setCheckable(true);
    connect(actionDelete, &QAction::triggered, this, &MainWindow::setModeDelete);

    actionUndo = new QAction("Deshacer", this);
    actionUndo->setShortcut(QKeySequence::Undo);
    connect(actionUndo, &QAction::triggered, this, &MainWindow::undo);

    actionRedo = new QAction("Rehacer", this);
    actionRedo->setShortcut(QKeySequence::Redo);
    connect(actionRedo, &QAction::triggered, this, &MainWindow::redo);

    actionForceLayout = new QAction("Organizar Automáticamente", this);
    connect(actionForceLayout, &QAction::triggered, this, &MainWindow::runForceLayout);
    
    // Grouping
    QActionGroup* modeGroup = new QActionGroup(this);
    modeGroup->addAction(actionAddNode);
    modeGroup->addAction(actionAddEdge);
    modeGroup->addAction(actionMove);
    modeGroup->addAction(actionDelete);
    actionMove->setChecked(true);
    
    // Algorithms
    actionPathMatrix = new QAction("Matriz de Caminos", this);
    connect(actionPathMatrix, &QAction::triggered, this, &MainWindow::runPathMatrix);
    
    actionDijkstra = new QAction("Dijkstra", this);
    connect(actionDijkstra, &QAction::triggered, this, &MainWindow::runDijkstra);
    
    actionFloyd = new QAction("Floyd-Warshall", this);
    connect(actionFloyd, &QAction::triggered, this, &MainWindow::runFloydWarshall);
    
    actionMST = new QAction("Árbol de Expansión Mínima (Prim)", this);
    connect(actionMST, &QAction::triggered, this, &MainWindow::runMST);
    
    actionDetectCycles = new QAction("Detectar Ciclos", this);
    connect(actionDetectCycles, &QAction::triggered, this, &MainWindow::runCycleDetection);

    actionCentrality = new QAction("Análisis de Centralidad", this);
    connect(actionCentrality, &QAction::triggered, this, &MainWindow::runCentralityAnalysis);
}

void MainWindow::createToolBar() {
    QToolBar* fileToolBar = addToolBar("Archivo");
    fileToolBar->addAction(actionNew);
    fileToolBar->addAction(actionLoad);
    fileToolBar->addAction(actionSave);
    fileToolBar->addAction(actionExportPNG);
    
    QMenu* fileMenu = menuBar()->addMenu("Archivo");
    fileMenu->addAction(actionNew);
    fileMenu->addAction(actionLoad);
    fileMenu->addAction(actionSave);
    fileMenu->addSeparator();
    fileMenu->addAction(actionExportPNG);
    
    QToolBar* editToolBar = addToolBar("Edición");
    editToolBar->addAction(actionMove);
    editToolBar->addAction(actionAddNode);
    editToolBar->addAction(actionAddEdge);
    editToolBar->addAction(actionDelete);
    editToolBar->addSeparator();
    editToolBar->addAction(actionUndo);
    editToolBar->addAction(actionRedo);
    editToolBar->addAction(actionForceLayout);
    
    QToolBar* algoToolBar = addToolBar("Algoritmos");
    algoToolBar->addAction(actionPathMatrix);
    algoToolBar->addAction(actionDijkstra);
    algoToolBar->addAction(actionFloyd);
    algoToolBar->addAction(actionMST);
    algoToolBar->addAction(actionDetectCycles);
    algoToolBar->addAction(actionCentrality);
}

void MainWindow::newGraph() {
    bool ok;
    QStringList items;
    items << "Dirigido, Pesado" << "Dirigido, No Pesado" << "No Dirigido, Pesado" << "No Dirigido, No Pesado";
    QString item = QInputDialog::getItem(this, "Nuevo Grafo", "Tipo:", items, 0, false, &ok);
    if (ok && !item.isEmpty()) {
        bool directed = item.contains("Dirigido") && !item.contains("No Dirigido");
        bool weighted = item.contains("Pesado") && !item.contains("No Pesado");
        graphView->setGraph(new Graph(directed, weighted));
        statusLabel->setText("Creado nuevo grafo " + item + ".");
    }
}

void MainWindow::loadGraph() {
    QString fileName = QFileDialog::getOpenFileName(this, "Cargar Grafo", "", "Archivos de Grafo (*.graph);;Todos los archivos (*)");
    if (fileName.isEmpty()) return;
    
    Graph* newGraph = FileController::loadGraph(fileName.toStdString());
    if (newGraph) {
        graphView->setGraph(newGraph);
        statusLabel->setText("Cargado " + fileName);
    } else {
        QMessageBox::critical(this, "Error", "No se pudo cargar el archivo.");
    }
}

void MainWindow::saveGraph() {
    QString fileName = QFileDialog::getSaveFileName(this, "Guardar Grafo", "", "Archivos de Grafo (*.graph);;Todos los archivos (*)");
    if (fileName.isEmpty()) return;
    
    if (FileController::saveGraph(graphView->getGraph(), fileName.toStdString())) {
        statusLabel->setText("Guardado en " + fileName);
    } else {
        QMessageBox::critical(this, "Error", "No se pudo guardar el archivo.");
    }
}

void MainWindow::exportPNG() {
    QString fileName = QFileDialog::getSaveFileName(this, "Exportar como Imagen", "", "PNG Files (*.png);;JPG Files (*.jpg)");
    if (fileName.isEmpty()) return;
    
    graphView->exportToImage(fileName);
    statusLabel->setText("Imagen exportada a " + fileName);
}


void MainWindow::setModeAddNode() {
    graphView->setInteractionMode(GraphView::Mode::AddNode);
    statusLabel->setText("Modo: Agregar Nodo (Click en espacio vacío)");
}

void MainWindow::setModeAddEdge() {
    graphView->setInteractionMode(GraphView::Mode::AddEdge);
    statusLabel->setText("Modo: Agregar Arista (Arrastra de un nodo a otro)");
}

void MainWindow::setModeMove() {
    graphView->setInteractionMode(GraphView::Mode::Move);
    statusLabel->setText("Modo: Mover/Seleccionar");
}

void MainWindow::setModeDelete() {
    graphView->setInteractionMode(GraphView::Mode::Delete);
    statusLabel->setText("Modo: Eliminar (Click en nodo o arista)");
}

void MainWindow::runForceLayout() {
    graphView->applyForceLayout();
    statusLabel->setText("Grafo organizado automáticamente.");
}

void MainWindow::runPathMatrix() {
    Graph* g = graphView->getGraph();
    int size;
    bool** matrix = GraphAlgorithms::getPathMatrix(g, size);
    
    QString result = "Matriz de Caminos (Alcanzabilidad):\n";
    for(int i=0; i<size; ++i) {
        for(int j=0; j<size; ++j) {
            result += (matrix[i][j] ? "1 " : "0 ");
        }
        result += "\n";
    }
    
    QMessageBox::information(this, "Matriz de Caminos", result);
    GraphAlgorithms::freeMatrix(matrix, size);
}

void MainWindow::runDijkstra() {
    bool ok;
    int startId = QInputDialog::getInt(this, "Dijkstra", "ID del Nodo Inicial:", 0, 0, 1000, 1, &ok);
    if(!ok) return;
    int endId = QInputDialog::getInt(this, "Dijkstra", "ID del Nodo Final:", 0, 0, 1000, 1, &ok);
    if(!ok) return;
    
    Graph* g = graphView->getGraph();
    LinkedList<Edge*> path = GraphAlgorithms::getShortestPathDijkstra(g, startId, endId);
    
    if (path.size() == 0) {
        QMessageBox::information(this, "Dijkstra", "No se encontró camino o hubo un error.");
    } else {
        graphView->highlightPath(path);
        QString msg = "Camino más corto:\n";
        for (auto e : path) {
            msg += QString::number(e->source->id) + " -> " + QString::number(e->dest->id) + " (" + QString::number(e->weight) + ")\n";
        }
        QMessageBox::information(this, "Resultado de Dijkstra", msg);
    }
}

void MainWindow::runFloydWarshall() {
    Graph* g = graphView->getGraph();
    int size;
    int* idMap;
    int** matrix = GraphAlgorithms::getAllPairsShortestPaths(g, size, idMap);
    
    QString result = "Floyd-Warshall (Distances):\n    ";
    for(int i=0; i<size; ++i) result += QString::number(idMap[i]) + " ";
    result += "\n";

    for(int i=0; i<size; ++i) {
        result += QString::number(idMap[i]) + ": ";
        for(int j=0; j<size; ++j) {
            if(matrix[i][j] > 1e8) result += "INF ";
            else result += QString::number(matrix[i][j]) + " ";
        }
        result += "\n";
    }
    
    QDialog* dlg = new QDialog(this);
    dlg->setWindowTitle("Resultado de la Matriz");
    QVBoxLayout* lay = new QVBoxLayout(dlg);
    QTextEdit* text = new QTextEdit(dlg);
    text->setText(result);
    text->setFontFamily("Courier");
    lay->addWidget(text);
    dlg->resize(400, 400);
    dlg->show(); // Non-modal or exec()
    
    GraphAlgorithms::freeMatrix(matrix, size);
    delete[] idMap;
}

void MainWindow::runMST() {
    Graph* g = graphView->getGraph();
    LinkedList<Edge*> mst = GraphAlgorithms::getMSTPrim(g);
    
    graphView->highlightPath(mst);
    QMessageBox::information(this, "MST", "MST resaltado. Total de aristas: " + QString::number(mst.size()));
}

void MainWindow::runCycleDetection() {
    if (GraphAlgorithms::hasCycles(graphView->getGraph())) {
        QMessageBox::warning(this, "Detección de Ciclos", "¡Se han detectado ciclos en el grafo!");
    } else {
        QMessageBox::information(this, "Detección de Ciclos", "El grafo no contiene ciclos.");
    }
}

void MainWindow::runCentralityAnalysis() {
    int size;
    int* idMap;
    double* closeness = GraphAlgorithms::getClosenessCentrality(graphView->getGraph(), idMap, size);
    
    if (!closeness) {
        QMessageBox::warning(this, "Centralidad", "No se pudo realizar el análisis.");
        return;
    }

    QString report = "Centralidad de Cercanía (Closeness):\n\n";
    for (int i = 0; i < size; ++i) {
        Vertex* v = graphView->getGraph()->getVertex(idMap[i]);
        if (v) report += QString("%1: %2\n").arg(QString::fromStdString(v->label)).arg(closeness[i], 0, 'f', 4);
    }

    QMessageBox::information(this, "Resultados de Centralidad", report);

    delete[] closeness;
    delete[] idMap;
}

void MainWindow::addCommand(Command* cmd) {
    undoStack.push_back(cmd);
    // Clear redo stack
    while(!redoStack.isEmpty()) {
        delete redoStack.back();
        redoStack.pop_back();
    }
}

void MainWindow::undo() {
    if (undoStack.isEmpty()) return;
    Command* cmd = undoStack.back();
    cmd->undo();
    undoStack.pop_back();
    redoStack.push_back(cmd);
    statusLabel->setText("Acción deshecha.");
}

void MainWindow::redo() {
    if (redoStack.isEmpty()) return;
    Command* cmd = redoStack.back();
    cmd->execute();
    redoStack.pop_back();
    undoStack.push_back(cmd);
    statusLabel->setText("Acción rehecha.");
}


void MainWindow::handleRequestAddNode(int id, QString label, qreal x, qreal y) {
    auto* cmd = new AddNodeCommand(graphView, id, label.toStdString(), x, y);
    addCommand(cmd);
    cmd->execute();
}

void MainWindow::handleRequestAddEdge(int srcId, int destId, int weight) {
    auto* cmd = new AddEdgeCommand(graphView, srcId, destId, weight);
    addCommand(cmd);
    cmd->execute();
}

void MainWindow::handleRequestDeleteVertex(int id) {
    auto* cmd = new DeleteNodeCommand(graphView, id);
    addCommand(cmd);
    cmd->execute();
}


void MainWindow::exportSVG() {
     QMessageBox::information(this, "Exportar", "La exportación a SVG no está disponible en este entorno (Falta QtSvg).");
}

void MainWindow::handleNodeMovedCommand(int id, QPointF oldPos, QPointF newPos) {
    addCommand(new MoveNodeCommand(graphView, id, oldPos, newPos));
}
