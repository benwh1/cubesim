#ifndef STICKER_H
#define STICKER_H

#include <QGraphicsPolygonItem>
#include <QVector3D>
#include "cube.h"
#include "projection.h"

class Sticker : public QGraphicsPolygonItem
{
public:
    Sticker(Cube::Face face, Projection *proj, qreal size, QGraphicsItem *parent = nullptr);

};

#endif // STICKER_H
