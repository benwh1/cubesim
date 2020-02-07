#include "sticker.h"

Sticker::Sticker(Face face, QPoint piecePos, Cube *cube, Settings *settings, Projection *proj, qreal size, QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent)
{
    this->face = face;
    this->piecePos = piecePos;
    this->cube = cube;
    this->settings = settings;
    this->size = size;

    //unit vectors pointing right/up when looking at the
    //face containing this sticker
    //e.g. if face is U, then upwards is from the F face to the B face
    //so up = (0,1,0) because the F -> B axis is the y axis, and travelling
    //from the F face to the B face increases the y coordinate
    QVector3D right, up;

    if(face == Face::U){
        right = QVector3D(1, 0, 0);
        up = QVector3D(0, 1, 0);
    }
    else if(face == Face::F){
        right = QVector3D(1, 0, 0);
        up = QVector3D(0, 0, 1);
    }
    else if(face == Face::R){
        right = QVector3D(0, 1, 0);
        up = QVector3D(0, 0, 1);
    }

    //coordinates of projections of right and up vectors;
    QPointF projRight = proj->project(right);
    QPointF projUp    = proj->project(up);

    //set the transform to map the square [0,1]^2 onto the projected sticker
    setTransform(QTransform(projRight.x(), projRight.y(), projUp.x(), projUp.y(), 0, 0));

    //create the polygon
    QPolygonF poly;
    poly << QPointF(0,0) << QPointF(0,1) << QPointF(1,1) << QPointF(1,0);
    for(int i=0; i<poly.size(); i++){
        poly[i] = poly[i] * size;
    }

    setPolygon(poly);
}

void Sticker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    //draw the basic polygon
    QGraphicsPolygonItem::paint(painter, option, widget);

    //if we're using a supercube, draw supercube stickers
    if(settings->getSupercube()){
        QString stickers = settings->getSupercubeStickers();

        if(stickers == "Arrows"){
            paintArrows(painter);
        }
        else if(stickers == "Pochmann"){
            paintPochmann(painter);
        }
        else if(stickers == "Image"){
            paintImage(painter);
        }
    }
}

void Sticker::paintArrows(QPainter *painter){
    QPolygonF arrow;
    arrow << QPointF(0.4, 0.1) << QPointF(0.4, 0.6) << QPointF(0.2, 0.6) << QPointF(0.5, 0.9) << QPointF(0.8, 0.6) << QPointF(0.6, 0.6) << QPointF(0.6, 0.1);
    for(int i=0; i<arrow.size(); i++){
        arrow[i] = arrow[i] * size;
    }

    //get the orientation of the sticker
    int orientation = cube->stickerOrientation(face, piecePos.x(), piecePos.y());

    //rotate the painter around the center of the sticker
    painter->scale(size, size);
    painter->translate(0.5, 0.5);
    painter->rotate(-90 * orientation);
    painter->translate(-0.5, -0.5);
    painter->scale(1/size, 1/size);

    //draw the arrow
    painter->drawPolygon(arrow);
}

void Sticker::paintPochmann(QPainter *painter){
    //the 4 faces adjacent to each face of the cube, in the order up right down left
    //e.g. looking at the U face in orientation 0, the adjacent faces are B, R, F, L
    QList<QList<Face>> adjacentFaces = {{Face::B, Face::R, Face::F, Face::L},  //U
                                        {Face::U, Face::R, Face::D, Face::L},  //F
                                        {Face::U, Face::B, Face::D, Face::F},  //R
                                        {Face::U, Face::L, Face::D, Face::R},  //B
                                        {Face::U, Face::F, Face::D, Face::B},  //L
                                        {Face::F, Face::R, Face::B, Face::L}}; //D

    //some useful constants
    int s = cube->getSize();
    qreal s2 = (s-1)/2.;
    int x = piecePos.x();
    int y = s-1-piecePos.y();

    //how wide are the pochmann sticker bars?
    qreal b = settings->getPochmannBarThickness();

    //sticker in the center
    if(s%2 == 1 && x == s/2 && y == s/2){
        //the trapezoidal shape of the bar
        QPolygonF p;
        p << QPointF(b, 1-b) << QPointF(0, 1) << QPointF(1, 1) << QPointF(1-b, 1-b);
        for(int i=0; i<p.size(); i++){
            p[i] = p[i] * size;
        }

        for(int i=0; i<4; i++){
            //which face does the sticker belong on?
            Face pieceFace = (Face)cube->sticker(face, piecePos.x(), piecePos.y());

            //orientation of the sticker
            int orientation = cube->stickerOrientation(face, piecePos.x(), piecePos.y());

            //the adjacent face, based on the stickers orientation
            Face adjacentFace = adjacentFaces[pieceFace][(4-orientation+i)%4];

            //colour of the adjacent face
            QColor adjacentColour = settings->getColour(adjacentFace);
            painter->setBrush(QBrush(adjacentColour));

            //draw the polygon
            painter->drawPolygon(p);

            //rotate the painter around the center of the sticker
            //in preparation for drawing the next bar
            painter->scale(size, size);
            painter->translate(0.5, 0.5);
            painter->rotate(-90);
            painter->translate(-0.5, -0.5);
            painter->scale(1/size, 1/size);
        }
    }
    //sticker on a diagonal, need to draw two bars instead of one
    else if(x == y || x == s-1-y){
        //two separate polygons, one for each colour
        QPolygonF p1, p2;
        p1 << QPointF(0, 0) << QPointF(0, 1) << QPointF(b, 1-b) << QPointF(b, 0);
        p2 << QPointF(b, 1-b) << QPointF(0, 1) << QPointF(1, 1) << QPointF(1, 1-b);
        for(int i=0; i<p1.size(); i++){
            p1[i] = p1[i] * size;
        }
        for(int i=0; i<p2.size(); i++){
            p2[i] = p2[i] * size;
        }

        //which diagonal is the sticker in?
        //top left = 0, top right = 1, bottom right = 2, bottom left = 3
        int diagonal;
             if(y == s-1-x && y > s2) diagonal = 0;
        else if(y == x     && y > s2) diagonal = 1;
        else if(y == s-1-x && y < s2) diagonal = 2;
        else if(y == x     && y < s2) diagonal = 3;

        //rotate the painter around the center of the sticker
        painter->scale(size, size);
        painter->translate(0.5, 0.5);
        painter->rotate(-90 * diagonal);
        painter->translate(-0.5, -0.5);
        painter->scale(1/size, 1/size);

        //which face does the sticker belong on?
        Face pieceFace = (Face)cube->sticker(face, piecePos.x(), piecePos.y());

        //orientation of the sticker
        int orientation = cube->stickerOrientation(face, piecePos.x(), piecePos.y());

        //the two adjacent faces, based on the stickers orientation
        Face adjacentFace1 = adjacentFaces[pieceFace][(4-orientation+diagonal+3)%4];
        Face adjacentFace2 = adjacentFaces[pieceFace][(4-orientation+diagonal)%4];

        //the colours of the adjacent faces
        QColor adjacentColour1 = settings->getColour(adjacentFace1);
        QColor adjacentColour2 = settings->getColour(adjacentFace2);

        painter->setBrush(QBrush(adjacentColour1));
        painter->drawPolygon(p1);
        painter->setBrush(QBrush(adjacentColour2));
        painter->drawPolygon(p2);
    }
    //normal sticker, only one bar
    else{
        //the sticker bar polygon
        QPolygonF p;
        p << QPointF(0, 1-b) << QPointF(0, 1) << QPointF(1, 1) << QPointF(1, 1-b);
        for(int i=0; i<p.size(); i++){
            p[i] = p[i] * size;
        }

        //which "wedge" is the sticker in?
        //top = 0, right = 1, bottom = 2, left = 3
        int wedge;

        //upper wedge
             if(  y-s2  > abs(x-s2)) wedge = 0;
        else if(  x-s2  > abs(y-s2)) wedge = 1;
        else if(-(y-s2) > abs(x-s2)) wedge = 2;
        else if(-(x-s2) > abs(y-s2)) wedge = 3;

        //rotate the painter around the center of the sticker
        painter->scale(size, size);
        painter->translate(0.5, 0.5);
        painter->rotate(-90 * wedge);
        painter->translate(-0.5, -0.5);
        painter->scale(1/size, 1/size);

        //which face does the sticker belong on?
        Face pieceFace = (Face)cube->sticker(face, piecePos.x(), piecePos.y());

        //orientation of the sticker
        int orientation = cube->stickerOrientation(face, piecePos.x(), piecePos.y());

        //the adjacent face, based on the stickers orientation
        Face adjacentFace = adjacentFaces[pieceFace][(4-orientation+wedge)%4];

        //the colour of the adjacent face - this is the colour of the pochmann sticker bar
        QColor adjacentColour = settings->getColour(adjacentFace);

        //set the brush colour
        painter->setBrush(QBrush(adjacentColour));

        //draw the bar
        painter->drawPolygon(p);
    }
}

void Sticker::paintImage(QPainter *painter){
    int orientation = cube->stickerOrientation(face, piecePos.x(), piecePos.y());

    //rotate the painter
    painter->scale(size, size);
    painter->translate(0.5, 0.5);
    painter->rotate(-90 * orientation);
    painter->scale(1, -1); //because the CubeGraphicsView is also flipped
    painter->translate(-0.5, -0.5);

    QPixmap pixmap("image.png");
    painter->drawPixmap(QRect(0,0,1,1), pixmap);
}
