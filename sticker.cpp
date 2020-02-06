#include "sticker.h"

Sticker::Sticker(Cube::Face face, Projection *proj, qreal size, QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent)
{
    this->size = size;

    //unit vectors pointing right/up when looking at the
    //face containing this sticker
    //e.g. if face is U, then upwards is from the F face to the B face
    //so up = (0,1,0) because the F -> B axis is the y axis, and travelling
    //from the F face to the B face increases the y coordinate
    QVector3D right, up;

    if(face == Cube::Face::U){
        right = QVector3D(1, 0, 0);
        up = QVector3D(0, 1, 0);
    }
    else if(face == Cube::F){
        right = QVector3D(1, 0, 0);
        up = QVector3D(0, 0, 1);
    }
    else if(face == Cube::R){
        right = QVector3D(0, 1, 0);
        up = QVector3D(0, 0, 1);
    }

    //coordinates of projections of right and up vectors;
    projRight = proj->project(right);
    projUp    = proj->project(up);

    //the bottom left corner of the sticker has coordinates (0,0)
    //in the sticker's coordinate system. the parent item is responsible
    //for moving the stickers of the cube into the correct position
    QPointF position(0, 0);

    //create the polygon
    QPolygonF poly;
    poly << position
         << position + size * (projUp)
         << position + size * (projUp + projRight)
         << position + size * (projRight);

    setPolygon(poly);

    //compute the center of the sticker
    //by linearity, we can just average two diagonally opposite corners
    center = (poly[0] + poly[2])/2;

    //by default, don't draw supercube stickers
    orientation = -1;
}

void Sticker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    //draw the basic polygon
    QGraphicsPolygonItem::paint(painter, option, widget);

    painter->scale(size, size);

    QList<QPointF> arrowPoints;
    arrowPoints << QPointF(0.4, 0.1) << QPointF(0.4, 0.6) << QPointF(0.2, 0.6) << QPointF(0.5, 0.9) << QPointF(0.8, 0.6) << QPointF(0.6, 0.6) << QPointF(0.6, 0.1);

    QPolygonF arrow;
    for(int i=0; i<arrowPoints.size(); i++){
             if(orientation == 0) arrow <<    arrowPoints[i].x()  * projRight +    arrowPoints[i].y()  * projUp;
        else if(orientation == 1) arrow <<    arrowPoints[i].y()  * projRight + (1-arrowPoints[i].x()) * projUp;
        else if(orientation == 2) arrow << (1-arrowPoints[i].x()) * projRight + (1-arrowPoints[i].y()) * projUp;
        else if(orientation == 3) arrow << (1-arrowPoints[i].y()) * projRight +    arrowPoints[i].x()  * projUp;
    }

    painter->drawPolygon(arrow);
}

void Sticker::setOrientation(int orientation){
    this->orientation = orientation;
}
