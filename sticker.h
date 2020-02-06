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
    Sticker(Cube::Face face, Projection *proj, qreal size, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // STICKER_H
