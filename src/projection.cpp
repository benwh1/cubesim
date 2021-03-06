#include "projection.h"

Projection::Projection(QMatrix3x2 m)
{
    matrix = m;
}

QPointF Projection::project(QVector3D v){
    return QPointF(matrix(0,0)*v.x() + matrix(0,1)*v.y() + matrix(0,2)*v.z(),
                   matrix(1,0)*v.x() + matrix(1,1)*v.y() + matrix(1,2)*v.z());
}

QVector3D Projection::unprojectU(QPointF p){
    //matrix.data returns the data column by column
    float *m = matrix.data();

    qreal u = p.x() - m[4];
    qreal v = p.y() - m[5];

    qreal a = m[0];
    qreal b = m[2];
    qreal c = m[1];
    qreal d = m[3];

    qreal det = a*d-b*c;

    //invert the matrix
    qreal x = (d*u-b*v)/det;
    qreal y = (-c*u+a*v)/det;
    qreal z = 1;

    return QVector3D(x, y, z);
}

QVector3D Projection::unprojectF(QPointF p){
    float *m = matrix.data();

    qreal u = p.x() + m[2];
    qreal v = p.y() + m[3];

    qreal a = m[0];
    qreal b = m[4];
    qreal c = m[1];
    qreal d = m[5];

    qreal det = a*d-b*c;

    //invert the matrix
    qreal x = (d*u-b*v)/det;
    qreal y = -1;
    qreal z = (-c*u+a*v)/det;

    return QVector3D(x, y, z);
}

QVector3D Projection::unprojectR(QPointF p){
    float *m = matrix.data();

    qreal u = p.x() - m[0];
    qreal v = p.y() - m[1];

    qreal a = m[2];
    qreal b = m[4];
    qreal c = m[3];
    qreal d = m[5];

    qreal det = a*d-b*c;

    //invert the matrix
    qreal x = 1;
    qreal y = (d*u-b*v)/det;
    qreal z = (-c*u+a*v)/det;

    return QVector3D(x, y, z);
}

QPolygonF Projection::unprojectU(QPolygonF p){
    QPolygonF poly;
    for(int i=0; i<p.size(); i++){
        QVector3D v = unprojectU(p.at(i));
        poly << QPointF(v.x(), v.y());
    }
    return poly;
}

QPolygonF Projection::unprojectF(QPolygonF p){
    QPolygonF poly;
    for(int i=0; i<p.size(); i++){
        QVector3D v = unprojectF(p.at(i));
        poly << QPointF(v.x(), v.z());
    }
    return poly;
}

QPolygonF Projection::unprojectR(QPolygonF p){
    QPolygonF poly;
    for(int i=0; i<p.size(); i++){
        QVector3D v = unprojectR(p.at(i));
        poly << QPointF(v.y(), v.z());
    }
    return poly;
}

QVector3D Projection::unproject(QPointF p, bool *ok){
    //unproject the point assuming it is on each of the three faces
    //U F or R, and if the point is on one of the faces, return the
    //unprojection of the point. if not, then the unprojected point
    //was outside of the cube [-1,1]^3

    QVector3D v;

    //try U face unprojection
    //if x and y are both in [-1,1], then the point was on the U face
    v = unprojectU(p);
    if(-1 <= v.x() && v.x() <= 1 && -1 <= v.y() && v.y() <= 1){
        *ok = true;
        return v;
    }

    //try F face unprojection
    //if x and z are both in [-1,1], then the point was on the F face
    v = unprojectF(p);
    if(-1 <= v.x() && v.x() <= 1 && -1 <= v.z() && v.z() <= 1){
        *ok = true;
        return v;
    }

    //try R face unprojection
    //if y and z are both in [-1,1], then the point was on the R face
    v = unprojectR(p);
    if(-1 <= v.y() && v.y() <= 1 && -1 <= v.z() && v.z() <= 1){
        *ok = true;
        return v;
    }

    //point was not on either of the faces
    *ok = false;
    return QVector3D(0, 0, 0);
}

QTransform Projection::toTransform(Face face){
    float *m = matrix.data();

    //matrix is {{a,b,c},{d,e,f}}
    qreal a = m[0];
    qreal b = m[2];
    qreal c = m[4];
    qreal d = m[1];
    qreal e = m[3];
    qreal f = m[5];

    //z = 1
    if(face == Face::U){
        return QTransform(a,d,b,e,0,0);
    }
    //y = -1
    else if(face == Face::F){
        return QTransform(a,d,c,f,0,0);
    }
    //x = 1
    else if(face == Face::R){
        return QTransform(b,e,c,f,0,0);
    }

    return QTransform();
}

bool Projection::isFaceVisible(Face face){
    float *m = matrix.data();

    //matrix is {{a,b,c},{d,e,f}}
    qreal a = m[0];
    qreal b = m[2];
    qreal c = m[4];
    qreal d = m[1];
    qreal e = m[3];
    qreal f = m[5];

    auto zero = [](qreal a){
        qreal epsilon = 1e-6;
        return qAbs(a) < epsilon;
    };

    if(face == Face::U){
        return !zero(a*e-b*d);
    }
    else if(face == Face::F){
        return !zero(a*f-c*d);
    }
    else if(face == Face::R){
        return !zero(b*f-c*e);
    }

    return false;
}

QJsonObject Projection::toJSON(){
    QJsonObject data;

    QJsonArray jsonMatrix;
    jsonMatrix.append(matrix(0,0));
    jsonMatrix.append(matrix(0,1));
    jsonMatrix.append(matrix(0,2));
    jsonMatrix.append(matrix(1,0));
    jsonMatrix.append(matrix(1,1));
    jsonMatrix.append(matrix(1,2));

    data["matrix"] = jsonMatrix;

    return data;
}

void Projection::fromJSON(QJsonObject data){
    float m[6];

    QJsonArray jsonMatrix = data["matrix"].toArray();
    for(int i=0; i<6; i++){
        m[i] = jsonMatrix[i].toDouble();
    }

    matrix = QMatrix3x2(m);
}
