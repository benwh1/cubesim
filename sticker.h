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

private:
    //which face of the cube is the sticker on?
    Cube::Face face;

    //edge length of the sticker in 3d space
    qreal size;

    Projection *proj;

};

#endif // STICKER_H
