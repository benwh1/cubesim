#ifndef STICKER_H
#define STICKER_H

#include <QGraphicsPolygonItem>
#include <QPainter>
#include <QVector3D>
#include "cube.h"
#include "projection.h"

class Sticker : public QGraphicsPolygonItem
{
public:
    Sticker(Cube::Face face, QPoint piecePos, Cube *cube, Projection *proj, qreal size, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Cube::Face face;
    QPoint piecePos;
    Cube *cube;

    qreal size;

    QPointF projRight;
    QPointF projUp;

    QPointF center;

};

#endif // STICKER_H
