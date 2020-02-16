#ifndef STICKER_H
#define STICKER_H

#include <QGraphicsPolygonItem>
#include <QPainter>
#include <QVector3D>
#include <cmath>
#include "cubestate.h"
#include "projection.h"
#include "settings.h"

class Sticker : public QGraphicsPolygonItem
{
public:
    Sticker(Face face, QPoint piecePos, CubeState *cube, Settings *settings, Projection *proj, qreal size, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Face face;
    QPoint piecePos;
    CubeState *cube;
    Settings *settings;

    //edge length of the sticker in 3d space
    qreal size;

    void paintArrows(QPainter *painter);
    void paintPochmann(QPainter *painter);
    void paintImage(QPainter *painter);

};

#endif // STICKER_H
