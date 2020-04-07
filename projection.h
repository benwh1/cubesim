#ifndef PROJECTION_H
#define PROJECTION_H

#include <QGenericMatrix>
#include <QJsonArray>
#include <QJsonObject>
#include <QPolygon>
#include <QTransform>
#include <QVector3D>
#include "enums.h"

class Projection
{
public:
    Projection(QMatrix3x2 m = QGenericMatrix<3,2,float>());

    //just a matrix vector product
    QPointF project(QVector3D v);

    /* suppose our matrix is {{a,b,c},{d,e,f}} and we are given a point {u,v}
     * suppose we want to unproject it to the U face of the cube [-1,1]^3,
     * i.e. the plane z = 1. then we have
     * {u,v}^T = {{a,b,c},{d,e,f}} . {x,y,1}^T
     *         = {{a,b},{d,e}} . {x,y}^T + {c,f}^T
     * so we can solve for x and y by subtracting {c,f}^T and inverting
     * the 2x2 matrix formed by removing the third column
     * the inverse of {{a,b},{c,d}} is 1/(ad-bc) {{d,-b},{-c,a}}
     * so {{a,b},{c,d}}^-1 . {u,v}^T = 1/(ad-bc) {du-bv, -cu+av}^T
     */

    //unproject a point assuming the original v was on the U face
    //the U face is the plane z = 1
    QVector3D unprojectU(QPointF p);

    //unproject a point assuming the original v was on the F face
    //the F face is the plane y = -1
    QVector3D unprojectF(QPointF p);

    //unproject a point assuming the original v was on the R face
    //the R face is the plane x = 1
    QVector3D unprojectR(QPointF p);

    //unproject polygons
    //return (x,y) where (x,y,1) are the unprojected coordinates
    QPolygonF unprojectU(QPolygonF p);

    //return (x,z) where (x,-1,z) are the unprojected coordinates
    QPolygonF unprojectF(QPolygonF p);

    //return (y,z) where (1,y,z) are the unprojected coordinates
    QPolygonF unprojectR(QPolygonF p);

    //inverse of project, assuming the original v was on either the
    //U F or R face of the cube [-1,1]^3
    QVector3D unproject(QPointF p, bool *ok = nullptr);

    //convert the projection to a QTransform for a given face. this is useful
    //because QTransform doesn't handle 3d -> 2d transformations.
    //if translate = false, the transform x -> Ax+b will have b = 0
    QTransform toTransform(Face face, bool translate = true);

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    QMatrix3x2 matrix;

};

#endif // PROJECTION_H
