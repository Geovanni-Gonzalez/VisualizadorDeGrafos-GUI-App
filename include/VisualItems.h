#ifndef VISUALITEMS_H
#define VISUALITEMS_H

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>
#include "Graph.h"

class GraphView; // Forward declaration

class VisualNode : public QGraphicsEllipseItem {
public:
    VisualNode(Vertex* v, GraphView* view);
    
    Vertex* getVertex() const { return vertex; }
    
    // Qt Events
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    
    // Update visual state (color, etc)
    void setHighlighted(bool high);

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    Vertex* vertex;
    GraphView* view;
    QPointF oldPos;
};

class VisualEdge : public QGraphicsLineItem {
public:
    VisualEdge(Edge* e, VisualNode* src, VisualNode* dest);
    
    Edge* getEdge() const { return edgeRaw; }
    void updatePosition();
    
    void setHighlighted(bool high);

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

private:
    Edge* edgeRaw;
    VisualNode* sourceNode;
    VisualNode* destNode;
    QGraphicsTextItem* weightLabel; // New member added
};

#endif // VISUALITEMS_H
