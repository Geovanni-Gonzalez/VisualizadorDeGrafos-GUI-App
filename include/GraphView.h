#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "Graph.h"
#include "VisualItems.h"
#include "LinkedList.h" // For highlight path

class GraphView : public QGraphicsView {
    Q_OBJECT

signals:
    void graphChanged();
    void nodeMovedCommand(int id, QPointF oldPos, QPointF newPos);
    void requestAddNode(int id, QString label, qreal x, qreal y);
    void requestAddEdge(int srcId, int destId, int weight);
    void requestDeleteVertex(int id);

public:
    enum class Mode {
        Move,
        AddNode,
        AddEdge,
        Delete
    };

    explicit GraphView(QWidget *parent = nullptr);
    ~GraphView();

    void setInteractionMode(Mode mode);
    Graph* getGraph();
    void setGraph(Graph* newGraph);
    
    void clear();
    void highlightPath(LinkedList<Edge*>& path);
    void exportToImage(const QString& fileName);
    void exportToSVG(const QString& fileName);
    void applyForceLayout();
    
    int getNextId() const { return nextId; }
    void setNextId(int id) { nextId = id; }
    
    // Public for VisualItems to call back
    void nodeClicked(VisualNode* node);
    void nodeMoved(VisualNode* node);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    QGraphicsScene* scene;
    Graph* graph;
    Mode currentMode;
    
    int nextId;
    VisualNode* tempSourceNode; // For creating edge
    
    // Mappings
    // We need to map Vertex* to VisualNode* for algorithms highlighting
    // Since we can't use std::map, we can search scene items or add lookup to Vertex? 
    // Adding `void* visualPtr` to Vertex is a hack but effective. 
    // Or just iterate scene items.
    VisualNode* getVisualForVertex(Vertex* v);
    VisualEdge* getVisualForEdge(Edge* e);

    bool isPanning;
    QPoint lastPanPos;
};

#endif // GRAPHVIEW_H
