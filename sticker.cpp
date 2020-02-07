#include "sticker.h"

Sticker::Sticker(Cube::Face face, QPoint piecePos, Cube *cube, Settings *settings, Projection *proj, qreal size, QGraphicsItem *parent) :
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
}

void Sticker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    //draw the basic polygon
    QGraphicsPolygonItem::paint(painter, option, widget);

    //if we're using a supercube, draw supercube stickers
    if(cube->isSupercube()){
        //the 4 faces adjacent to each face of the cube, in the order up right down left
        //e.g. looking at the U face in orientation 0, the adjacent faces are B, R, F, L
        QList<QList<Cube::Face>> adjacentFaces = {{Cube::Face::B, Cube::Face::R, Cube::Face::F, Cube::Face::L},  //U
                                                  {Cube::Face::U, Cube::Face::R, Cube::Face::D, Cube::Face::L},  //F
                                                  {Cube::Face::U, Cube::Face::B, Cube::Face::D, Cube::Face::F},  //R
                                                  {Cube::Face::U, Cube::Face::L, Cube::Face::D, Cube::Face::R},  //B
                                                  {Cube::Face::U, Cube::Face::F, Cube::Face::D, Cube::Face::B},  //L
                                                  {Cube::Face::F, Cube::Face::R, Cube::Face::B, Cube::Face::L}}; //D

        int s = cube->getSize();
        qreal s2 = (s-1)/2.;
        int x = piecePos.x();
        int y = s-1-piecePos.y();

        //how wide are the pochmann sticker bars?
        qreal barWidth = 0.2;

        //sticker in the center
        if(s%2 == 1 && x == s/2 && y == s/2){
            QPolygonF p1, p2, p3, p4;
            p1 << barWidth * projRight + (1 - barWidth) * projUp
               << 0.0 * projRight + 1.0 * projUp
               << 1.0 * projRight + 1.0 * projUp
               << (1 - barWidth) * projRight + (1 - barWidth) * projUp;
            p2 << (1 - barWidth) * projRight + barWidth * projUp
               << (1 - barWidth) * projRight + (1 - barWidth) * projUp
               << 1.0 * projRight + 1.0 * projUp
               << 1.0 * projRight + 0.0 * projUp;
            p3 << 0.0 * projRight + 0.0 * projUp
               << barWidth * projRight + barWidth * projUp
               << (1 - barWidth) * projRight + barWidth * projUp
               << 1.0 * projRight + 0.0 * projUp;
            p4 << 0.0 * projRight + 0.0 * projUp
               << 0.0 * projRight + 1.0 * projUp
               << barWidth * projRight + (1 - barWidth) * projUp
               << barWidth * projRight + barWidth * projUp;

            //which face does the sticker belong on?
            Cube::Face pieceFace = (Cube::Face)cube->sticker(face, piecePos.x(), piecePos.y());

            //orientation of the sticker
            int orientation = cube->stickerOrientation(face, piecePos.x(), piecePos.y());

            //the two adjacent faces, based on the stickers orientation
            Cube::Face adjacentFace1 = adjacentFaces[pieceFace][(4-orientation)%4];
            Cube::Face adjacentFace2 = adjacentFaces[pieceFace][(4-orientation+1)%4];
            Cube::Face adjacentFace3 = adjacentFaces[pieceFace][(4-orientation+2)%4];
            Cube::Face adjacentFace4 = adjacentFaces[pieceFace][(4-orientation+3)%4];

            //the colours of the adjacent faces
            QColor adjacentColour1 = settings->getColour(adjacentFace1);
            QColor adjacentColour2 = settings->getColour(adjacentFace2);
            QColor adjacentColour3 = settings->getColour(adjacentFace3);
            QColor adjacentColour4 = settings->getColour(adjacentFace4);

            //scale the painter up since our coordinates are in [0,1]^2
            painter->scale(size, size);

            painter->setBrush(QBrush(adjacentColour1));
            painter->drawPolygon(p1);
            painter->setBrush(QBrush(adjacentColour2));
            painter->drawPolygon(p2);
            painter->setBrush(QBrush(adjacentColour3));
            painter->drawPolygon(p3);
            painter->setBrush(QBrush(adjacentColour4));
            painter->drawPolygon(p4);
        }
        //sticker on a diagonal, need to draw two bars instead of one
        else if(x == y || x == s-1-y){
            //two separate polygons, one for each colour
            QPolygonF p1, p2;

            //which diagonal is the sticker in?
            //top left = 0, top right = 1, bottom right = 2, bottom left = 3
            int diagonal;

            if(y == s-1-x && y > s2){
                p1 <<      0.0 * projRight +            0.0 * projUp
                   <<      0.0 * projRight +            1.0 * projUp
                   << barWidth * projRight + (1 - barWidth) * projUp
                   << barWidth * projRight +            0.0 * projUp;
                p2 << barWidth * projRight + (1 - barWidth) * projUp
                   <<      0.0 * projRight +            1.0 * projUp
                   <<      1.0 * projRight +            1.0 * projUp
                   <<      1.0 * projRight + (1 - barWidth) * projUp;
                diagonal = 0;
            }
            else if(y == x && y > s2){
                p1 <<            0.0 * projRight + (1 - barWidth) * projUp
                   <<            0.0 * projRight +            1.0 * projUp
                   <<            1.0 * projRight +            1.0 * projUp
                   << (1 - barWidth) * projRight + (1 - barWidth) * projUp;
                p2 << (1 - barWidth) * projRight +            0.0 * projUp
                   << (1 - barWidth) * projRight + (1 - barWidth) * projUp
                   <<            1.0 * projRight +            1.0 * projUp
                   <<            1.0 * projRight +            0.0 * projUp;
                diagonal = 1;
            }
            else if(y == s-1-x && y < s2){
                p1 << (1 - barWidth) * projRight + barWidth * projUp
                   << (1 - barWidth) * projRight +      1.0 * projUp
                   <<            1.0 * projRight +      1.0 * projUp
                   <<            1.0 * projRight +      0.0 * projUp;
                p2 <<            0.0 * projRight +      0.0 * projUp
                   <<            0.0 * projRight + barWidth * projUp
                   << (1 - barWidth) * projRight + barWidth * projUp
                   <<            1.0 * projRight +      0.0 * projUp;
                diagonal = 2;
            }
            else if(y == x && y < s2){
                p1 <<      0.0 * projRight +      0.0 * projUp
                   << barWidth * projRight + barWidth * projUp
                   <<      1.0 * projRight + barWidth * projUp
                   <<      1.0 * projRight +      0.0 * projUp;
                p2 <<      0.0 * projRight +      0.0 * projUp
                   <<      0.0 * projRight +      1.0 * projUp
                   << barWidth * projRight +      1.0 * projUp
                   << barWidth * projRight + barWidth * projUp;
                diagonal = 3;
            }

            //which face does the sticker belong on?
            Cube::Face pieceFace = (Cube::Face)cube->sticker(face, piecePos.x(), piecePos.y());

            //orientation of the sticker
            int orientation = cube->stickerOrientation(face, piecePos.x(), piecePos.y());

            //the two adjacent faces, based on the stickers orientation
            Cube::Face adjacentFace1 = adjacentFaces[pieceFace][(4-orientation+diagonal+3)%4];
            Cube::Face adjacentFace2 = adjacentFaces[pieceFace][(4-orientation+diagonal)%4];

            //the colours of the adjacent faces
            QColor adjacentColour1 = settings->getColour(adjacentFace1);
            QColor adjacentColour2 = settings->getColour(adjacentFace2);

            //scale the painter up since our coordinates are in [0,1]^2
            painter->scale(size, size);

            painter->setBrush(QBrush(adjacentColour1));
            painter->drawPolygon(p1);
            painter->setBrush(QBrush(adjacentColour2));
            painter->drawPolygon(p2);
        }
        //normal sticker, only one bar
        else{
            //the sticker bar polygon
            QPolygonF p;

            //which "wedge" is the sticker in?
            //top = 0, right = 1, bottom = 2, left = 3
            int wedge;

            //upper wedge
            if(y-s2 > abs(x-s2)){
                p << (1 - barWidth) * projUp + 0.0 * projRight
                  <<            1.0 * projUp + 0.0 * projRight
                  <<            1.0 * projUp + 1.0 * projRight
                  << (1 - barWidth) * projUp + 1.0 * projRight;
                wedge = 0;
            }
            else if(x-s2 > abs(y-s2)){
                p << (1 - barWidth) * projRight + 0.0 * projUp
                  << (1 - barWidth) * projRight + 1.0 * projUp
                  <<            1.0 * projRight + 1.0 * projUp
                  <<            1.0 * projRight + 0.0 * projUp;
                wedge = 1;
            }
            else if(-(y-s2) > abs(x-s2)){
                p << 0.0 * projRight +      0.0 * projUp
                  << 0.0 * projRight + barWidth * projUp
                  << 1.0 * projRight + barWidth * projUp
                  << 1.0 * projRight +      0.0 * projUp;
                wedge = 2;
            }
            else if(-(x-s2) > abs(y-s2)){
                p <<      0.0 * projRight + 0.0 * projUp
                  <<      0.0 * projRight + 1.0 * projUp
                  << barWidth * projRight + 1.0 * projUp
                  << barWidth * projRight + 0.0 * projUp;
                wedge = 3;
            }
            else return;

            //which face does the sticker belong on?
            Cube::Face pieceFace = (Cube::Face)cube->sticker(face, piecePos.x(), piecePos.y());

            //orientation of the sticker
            int orientation = cube->stickerOrientation(face, piecePos.x(), piecePos.y());

            //the adjacent face, based on the stickers orientation
            Cube::Face adjacentFace = adjacentFaces[pieceFace][(4-orientation+wedge)%4];

            //the colour of the adjacent face - this is the colour of the pochmann sticker bar
            QColor adjacentColour = settings->getColour(adjacentFace);

            //set the brush colour
            painter->setBrush(QBrush(adjacentColour));

            //scale the painter up since our coordinates are in [0,1]^2
            painter->scale(size, size);

            //draw the bar
            painter->drawPolygon(p);
        }
    }
}
