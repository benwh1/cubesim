#ifndef CUBEGRAPHICSOBJECT_H
#define CUBEGRAPHICSOBJECT_H

#include <QBrush>
#include <QDebug>
#include <QGraphicsObject>
#include <QGraphicsSceneDragDropEvent>
#include <QPen>
#include <QVector3D>
#include <cmath>
#include "cube.h"
#include "projection.h"

#define pi (4*atan(1))

class CubeGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
public:
    enum Direction{
        Up, Left, Down, Right
    };

    explicit CubeGraphicsObject(Cube *c = nullptr, QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

    void setEdgeLength(qreal edgeLength);
    void setGapSize(qreal gapSize);

    void setProjection(float *mat);

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Cube *cube;
    QList<QList<QList<QGraphicsPolygonItem*>>> stickers;
    QList<QColor> colours;

    qreal edgeLength;
    qreal gapSize;

    Projection proj;

    QPointF lastMousePress;
    QPointF lastMouseRelease;

private slots:
    void reset();
    void updateSticker(Cube::Face face, int x, int y);
    void updateFace(Cube::Face face);
    void updateLayer(Cube::Axis axis, int layer);
    void updateAll();

signals:
    void projectionChanged();
    void moveDrag(Cube::Axis axis, int layer, bool clockwise);

};

#endif // CUBEGRAPHICSOBJECT_H
