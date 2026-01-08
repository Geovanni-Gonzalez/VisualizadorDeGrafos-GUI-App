#include "GraphView.h"
#include <QMouseEvent>
#include <QGraphicsTextItem>
#include <cmath>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QScrollBar>

// ================= VisualNode =================
VisualNode::VisualNode(Vertex* v, GraphView* view) 
    : vertex(v), view(view) 
{
    setRect(-15, -15, 30, 30);
    
    QRadialGradient gradient(0, 0, 15, -5, -5);
    gradient.setColorAt(0, QColor(255, 255, 255));
    gradient.setColorAt(1, QColor(200, 205, 210));
    setBrush(gradient);
    
    setPen(QPen(QColor(40, 42, 45, 180), 1.5));
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    
    // Premium Drop Shadow
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(15);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 120));
    setGraphicsEffect(shadow);
    
    // Label - cleaner look
    QGraphicsTextItem* text = new QGraphicsTextItem(QString::fromStdString(v->label), this);
    text->setDefaultTextColor(QColor(240, 240, 240));
    QFont font("Segoe UI", 9, QFont::Bold);
    text->setFont(font);
    text->setPos(-10, -38);
    
    setPos(v->x, v->y);
}

void VisualNode::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    oldPos = pos();
    view->nodeClicked(this);
    QGraphicsEllipseItem::mousePressEvent(event);
}

void VisualNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsEllipseItem::mouseMoveEvent(event);
    view->nodeMoved(this);
}

void VisualNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsEllipseItem::mouseReleaseEvent(event);
    if(pos() != oldPos) {
        view->nodeMovedCommand(vertex->id, oldPos, pos());
    }
}

QVariant VisualNode::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        if(view) view->nodeMoved(this);
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

void VisualNode::setHighlighted(bool high) {
    if(high) {
        QRadialGradient highGrad(0, 0, 15, -5, -5);
        highGrad.setColorAt(0, QColor(100, 255, 100));
        highGrad.setColorAt(1, QColor(0, 150, 0));
        setBrush(highGrad);
        setPen(QPen(Qt::white, 2));
    } else {
        QRadialGradient normGrad(0, 0, 15, -5, -5);
        normGrad.setColorAt(0, QColor(240, 240, 240));
        normGrad.setColorAt(1, QColor(180, 180, 180));
        setBrush(normGrad);
        setPen(QPen(QColor(60, 60, 60), 1.5));
    }
}


// ================= VisualEdge =================
VisualEdge::VisualEdge(Edge* e, VisualNode* src, VisualNode* dest) 
    : edgeRaw(e), sourceNode(src), destNode(dest)
{
    setPen(QPen(QColor(160, 160, 160), 2));
    setZValue(-1); // Behind nodes
    
    weightLabel = new QGraphicsTextItem(this);
    weightLabel->setDefaultTextColor(QColor(180, 180, 180));
    QFont font = weightLabel->font();
    font.setPointSize(8);
    font.setBold(true);
    weightLabel->setFont(font);
    
    weightLabel->setPlainText(QString::number(e->weight));

    updatePosition();
}

void VisualEdge::updatePosition() {
    if (!sourceNode || !destNode) return;
    QLineF line(sourceNode->pos(), destNode->pos());
    setLine(line);
    
    // Update label position at center
    QPointF center = line.center();
    weightLabel->setPos(center.x() - weightLabel->boundingRect().width()/2, center.y() - weightLabel->boundingRect().height()/2);
    
    // Rotate label to follow line? (Optional, maybe keep horizontal for readability)
    // For now keep it simple.
}

void VisualEdge::setHighlighted(bool high) {
    if(high) setPen(QPen(QColor(255, 69, 0), 4)); // Orange red for highlighting
    else setPen(QPen(QColor(160, 160, 160), 2));
}


// ================= GraphView =================
GraphView::GraphView(QWidget *parent) : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 2000, 2000);
    setScene(scene);
    
    graph = new Graph(true, true); // Default directed, weighted
    currentMode = Mode::Move;
    nextId = 1;
    tempSourceNode = nullptr;
    
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::RubberBandDrag);
    isPanning = false;
    
    // Modern Canvas Background with subtle grid
    setBackgroundBrush(QBrush(QColor(26, 28, 30)));
    
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setStyleSheet("border: none; border-radius: 12px; background-color: #1a1c1e;");
}

void GraphView::drawBackground(QPainter *painter, const QRectF &rect) {
    QGraphicsView::drawBackground(painter, rect);
    
    // Draw subtle grid
    painter->setPen(QPen(QColor(255, 255, 255, 15), 0.5));
    qreal left = int(rect.left()) - (int(rect.left()) % 50);
    qreal top = int(rect.top()) - (int(rect.top()) % 50);
    
    for (qreal x = left; x < rect.right(); x += 50)
        painter->drawLine(x, rect.top(), x, rect.bottom());
    for (qreal y = top; y < rect.bottom(); y += 50)
        painter->drawLine(rect.left(), y, rect.right(), y);
}

GraphView::~GraphView() {
    delete graph;
}

Graph* GraphView::getGraph() {
    return graph;
}

void GraphView::setGraph(Graph* newGraph) {
    if (!newGraph) return;
    
    clear();
    delete graph;
    graph = newGraph;
    
    // Reconstruct visual items
    // First pass: Nodes
    for (auto v : graph->getVertices()) {
        VisualNode* vn = new VisualNode(v, this);
        scene->addItem(vn);
        if (v->id >= nextId) nextId = v->id + 1;
    }
    
    // Second pass: Edges
    for (auto v : graph->getVertices()) {
        VisualNode* srcVN = getVisualForVertex(v);
        for (auto e : v->edges) {
            VisualNode* destVN = getVisualForVertex(e->dest);
            if (srcVN && destVN) {
                VisualEdge* ve = new VisualEdge(e, srcVN, destVN);
                scene->addItem(ve);
            }
        }
    }
    emit graphChanged();
}

void GraphView::setInteractionMode(Mode mode) {
    currentMode = mode;
    tempSourceNode = nullptr;
    if (mode == Mode::Move) {
        setDragMode(QGraphicsView::RubberBandDrag);
    } else {
        setDragMode(QGraphicsView::NoDrag);
    }
}

void GraphView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        isPanning = true;
        lastPanPos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        return;
    }

    if (currentMode == Mode::AddNode) {
        QPointF setPos = mapToScene(event->pos());
        int id = nextId++;
        QString label = "Nodo " + QString::number(id);
        emit requestAddNode(id, label, setPos.x(), setPos.y());
    } 
    // Other modes handled by VisualNode::mousePressEvent mostly, 
    // unless clicking on empty space to clear selection.
    QGraphicsView::mousePressEvent(event);
}

void GraphView::mouseMoveEvent(QMouseEvent *event) {
    if (isPanning) {
        QPoint delta = event->pos() - lastPanPos;
        lastPanPos = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GraphView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphView::wheelEvent(QWheelEvent *event) {
    const qreal factor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(factor, factor);
    } else {
        scale(1.0 / factor, 1.0 / factor);
    }
}

void GraphView::nodeClicked(VisualNode* node) {
    if (currentMode == Mode::AddEdge) {
        if (tempSourceNode == nullptr) {
            tempSourceNode = node;
            node->setHighlighted(true); // Temporary visual feedback
        } else {
            if (tempSourceNode != node) {
                emit requestAddEdge(tempSourceNode->getVertex()->id, node->getVertex()->id, 1);
            }
            tempSourceNode->setHighlighted(false);
            tempSourceNode = nullptr;
        }
    } else if (currentMode == Mode::Delete) {
        emit requestDeleteVertex(node->getVertex()->id);
    }
}

void GraphView::nodeMoved(VisualNode* node) {
    // Update all connected edges
    // Expensive to iterate all items. optimize by storing edges in VisualNode?
    QList<QGraphicsItem*> items = scene->items();
    for(auto item : items) {
        if(item->type() == VisualEdge::Type) {
            VisualEdge* ve = static_cast<VisualEdge*>(item);
            ve->updatePosition();
        }
    }
}

void GraphView::clear() {
    scene->clear();
    graph->clear();
    nextId = 1;
    emit graphChanged();
}

VisualNode* GraphView::getVisualForVertex(Vertex* v) {
    QList<QGraphicsItem*> items = scene->items();
    for(auto item : items) {
        if(item->type() == VisualNode::Type) {
            VisualNode* vn = static_cast<VisualNode*>(item);
            if(vn->getVertex() == v) return vn;
        }
    }
    return nullptr;
}

VisualEdge* GraphView::getVisualForEdge(Edge* e) {
    QList<QGraphicsItem*> items = scene->items();
    for(auto item : items) {
        if(item->type() == VisualEdge::Type) {
            VisualEdge* ve = static_cast<VisualEdge*>(item);
            if(ve->getEdge() == e) return ve;
        }
    }
    // Also check reverse edge if undirected? 
    return nullptr;
}

void GraphView::highlightPath(LinkedList<Edge*>& path) {
    // Reset all
    QList<QGraphicsItem*> items = scene->items();
    for(auto item : items) {
        if(item->type() == VisualNode::Type) {
            static_cast<VisualNode*>(item)->setHighlighted(false);
        } else if(item->type() == VisualEdge::Type) {
            static_cast<VisualEdge*>(item)->setHighlighted(false);
        }
    }
    
    // Highlight
    for(auto e : path) {
        VisualEdge* ve = getVisualForEdge(e);
        if(ve) {
            ve->setHighlighted(true);
        }
    }
}

void GraphView::exportToImage(const QString& fileName) {
    scene->clearSelection();
    QRectF rect = scene->itemsBoundingRect();
    if(rect.isEmpty()) rect = scene->sceneRect();
    
    rect.adjust(-20, -20, 20, 20);
    
    QImage image(rect.size().toSize(), QImage::Format_ARGB32);
    image.fill(QColor(26, 28, 30));
    
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter, QRectF(), rect);
    painter.end();
    
    image.save(fileName);
}

void GraphView::applyForceLayout() {
    int iterations = 100;
    qreal area = 1500 * 1500;
    int vCount = graph->getVertices().size();
    if(vCount == 0) return;
    qreal k = std::sqrt(area / vCount);
    
    for(int iter = 0; iter < iterations; ++iter) {
        qreal* dispX = new qreal[vCount];
        qreal* dispY = new qreal[vCount];
        for(int i=0; i<vCount; i++) dispX[i] = dispY[i] = 0;
        
        int* idMap = new int[vCount];
        int idx = 0;
        for(auto v : graph->getVertices()) idMap[idx++] = v->id;

        int i = 0;
        for(auto v : graph->getVertices()) {
            for(auto u : graph->getVertices()) {
                if(v != u) {
                    qreal dx = v->x - u->x;
                    qreal dy = v->y - u->y;
                    qreal dist = std::sqrt(dx*dx + dy*dy);
                    if(dist < 0.1) dist = 0.1;
                    qreal rf = (k*k) / dist;
                    dispX[i] += (dx/dist) * rf;
                    dispY[i] += (dy/dist) * rf;
                }
            }
            i++;
        }

        i = 0;
        for(auto v : graph->getVertices()) {
            for(auto e : v->edges) {
                Vertex* u = e->dest;
                qreal dx = v->x - u->x;
                qreal dy = v->y - u->y;
                qreal dist = std::sqrt(dx*dx + dy*dy);
                if(dist < 0.1) dist = 0.1;
                qreal af = (dist*dist) / k;
                
                int uIdx = -1;
                for(int m=0; m<vCount; m++) if(idMap[m] == u->id) { uIdx = m; break; }

                if(uIdx != -1) {
                    dispX[i] -= (dx/dist) * af;
                    dispY[i] -= (dy/dist) * af;
                    dispX[uIdx] += (dx/dist) * af;
                    dispY[uIdx] += (dy/dist) * af;
                }
            }
            i++;
        }

        i = 0;
        qreal temp = 50.0 / (iter + 1);
        for(auto v : graph->getVertices()) {
            qreal dist = std::sqrt(dispX[i]*dispX[i] + dispY[i]*dispY[i]);
            if(dist > 0.1) {
                qreal limitedDist = std::min(dist, temp);
                v->x += (dispX[i]/dist) * limitedDist;
                v->y += (dispY[i]/dist) * limitedDist;
            }
            i++;
        }

        delete[] dispX;
        delete[] dispY;
        delete[] idMap;
    }

    for(auto item : scene->items()) {
        VisualNode* vn = qgraphicsitem_cast<VisualNode*>(item);
        if(vn) {
            vn->setPos(vn->getVertex()->x, vn->getVertex()->y);
            nodeMoved(vn);
        }
    }
}
