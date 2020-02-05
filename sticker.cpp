#include "sticker.h"

Sticker::Sticker(Cube::Face face, Projection *proj, qreal size, QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent)
{
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
    QPointF projRight = proj->project(right);
    QPointF projUp    = proj->project(up);

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
}
