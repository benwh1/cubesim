#ifndef STICKER_H
#define STICKER_H

#include <QGraphicsRectItem>
#include <QPainter>
#include <QVector3D>
#include <cmath>
#include "cube.h"
#include "projection.h"
#include "settings.h"

class Sticker : public QGraphicsRectItem
{
public:
    Sticker(Face face, QPoint piecePos, Cube *cube, Settings *settings, Projection *proj, qreal size, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Face face;
    QPoint piecePos;
    Cube *cube;
    Settings *settings;

    //edge length of the sticker in 3d space
    qreal size;

    void paintArrows(QPainter *painter);
    void paintPochmann(QPainter *painter);
    void paintImage(QPainter *painter);

};

#endif // STICKER_H
