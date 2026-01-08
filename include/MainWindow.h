#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QComboBox>
#include <QLabel>
#include "GraphView.h"
#include "Graph.h"
#include "Command.h"
#include "LinkedList.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGraph();
    void loadGraph();
    void saveGraph();
    void exportPNG();
    void exportSVG();
    
    // Tools
    void setModeAddNode();
    void setModeAddEdge();
    void setModeMove();
    void setModeDelete();
    void runForceLayout();
    
    // Algorithms
    void runPathMatrix();
    void runDijkstra();
    void runFloydWarshall();
    void runMST();
    void runCycleDetection();
    void runCentralityAnalysis();
    
    void undo();
    void redo();
    void addCommand(Command* cmd);
    void handleNodeMovedCommand(int id, QPointF oldPos, QPointF newPos);
    void handleRequestAddNode(int id, QString label, qreal x, qreal y);
    void handleRequestAddEdge(int srcId, int destId, int weight);
    void handleRequestDeleteVertex(int id);
    
    void updateStats();

private:
    void createActions();
    void createToolBar();
    void applyStyle();

    GraphView* graphView;
    QLabel* statsLabel;
    
    QAction* actionNew;
    QAction* actionLoad;
    QAction* actionSave;
    QAction* actionExportPNG;
    
    QAction* actionAddNode;
    QAction* actionAddEdge;
    QAction* actionMove;
    QAction* actionDelete;
    QAction* actionForceLayout;
    
    QAction* actionPathMatrix;
    QAction* actionDijkstra;
    QAction* actionFloyd;
    QAction* actionMST;
    QAction* actionDetectCycles;
    QAction* actionCentrality;
    
    QAction* actionUndo;
    QAction* actionRedo;
    
    LinkedList<Command*> undoStack;
    LinkedList<Command*> redoStack;

    QLabel* statusLabel;
};

#endif // MAINWINDOW_H
