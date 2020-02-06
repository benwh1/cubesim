#ifndef STICKER_H
#define STICKER_H

#include <QGraphicsPolygonItem>
#include <QPainter>
#include <QVector3D>
#include <cmath>
#include "cube.h"
#include "projection.h"
#include "settings.h"

class Sticker : public QGraphicsPolygonItem
{
public:
    Sticker(Cube::Face face, QPoint piecePos, Cube *cube, Settings *settings, Projection *proj, qreal size, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Cube::Face face;
    QPoint piecePos;
    Cube *cube;
    Settings *settings;

    qreal size;

    QPointF projRight;
    QPointF projUp;

    QPointF center;

};

#endif // STICKER_H
