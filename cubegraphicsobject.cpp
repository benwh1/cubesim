#include "cubegraphicsobject.h"

CubeGraphicsObject::CubeGraphicsObject(Cube *c, QGraphicsObject *parent) :
    QGraphicsObject(parent)
{
    cube = c;

    connect(cube, SIGNAL(moveDone(Cube::Axis,int,int,int)), this, SLOT(onMoveDone(Cube::Axis,int,int,int)));
    connect(cube, SIGNAL(rotationDone(Cube::Axis,int)), this, SLOT(updateAll()));
    connect(cube, SIGNAL(cubeReset()), this, SLOT(updateAll()));
    connect(cube, SIGNAL(cubeScrambled()), this, SLOT(updateAll()));
    connect(cube, SIGNAL(cubeSizeChanged()), this, SLOT(reset()));

    float mat[6] = {1/sqrt(2), 1/sqrt(2), 0, -1/sqrt(6), 1/sqrt(6), sqrt(2./3)};
    proj = Projection(QMatrix3x2(mat));

    colours << QColor(Qt::white)
            << QColor(Qt::green)
            << QColor(Qt::red)
            << QColor(Qt::blue)
            << QColor(255,165,0)
            << QColor(Qt::yellow);

    setEdgeLength(400);
    setGapSize(0);

    reset();
}

QRectF CubeGraphicsObject::boundingRect() const{
    return childrenBoundingRect();
}

void CubeGraphicsObject::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *){
    //nothing to do, all the children are drawn automatically
}

void CubeGraphicsObject::reset(){
    for(int face=0; face<stickers.size(); face++){
        for(int y=0; y<stickers[face].size(); y++){
            for(int x=0; x<stickers[face][y].size(); x++){
                delete stickers[face][y][x];
            }
        }
    }

    stickers.clear();

    for(int face=0; face<3; face++){
        stickers.append(QList<QList<QGraphicsPolygonItem*>>());
        for(int y=0; y<cube->getSize(); y++){
            stickers[face].append(QList<QGraphicsPolygonItem*>());
            for(int x=0; x<cube->getSize(); x++){
                QGraphicsPolygonItem *sticker = new QGraphicsPolygonItem(this);
                stickers[face][y].append(sticker);
            }
        }
    }

    //some constants
    int s = cube->getSize();
    //float stickerSize = (edgeLength - (s-1) * gapSize)/s;
    //float delta = ((edgeLength - (s-1) * gapSize)/s + gapSize)/edgeLength;
    //simplified above formulas to reduce floating point errors
    float stickerSize = (edgeLength+gapSize)/s - gapSize;
    float delta = 1.0f/s + gapSize/(edgeLength*s);

    QVector<QVector3D> basis = {QVector3D(1,0,0), QVector3D(0,1,0), QVector3D(0,0,1)};
    QVector<QPointF> basisImage;

    for(int i=0; i<3; i++){
        basisImage.append(proj.project(basis[i]));
    }

    //U face
    for(int y=0; y<s; y++){
        for(int x=0; x<s; x++){
            //the bottom left corner of the sticker on the cube [0,1]^3
            QVector3D v(x*delta, (s-1-y)*delta, 1);

            //scale the point to be in [-1,1]^3 instead of [0,1]^3
            v = 2 * v - QVector3D(1, 1, 1);

            //compute the isometric projection of the corner of the sticker
            //and scale it up so the cube has the required edge length
            QPointF point = edgeLength/2 * proj.project(v);

            QPolygonF poly;
            poly << point
                 << point + stickerSize * (basisImage[0])
                 << point + stickerSize * (basisImage[0] + basisImage[1])
                 << point + stickerSize * (basisImage[1])
                 << point;

            QGraphicsPolygonItem *sticker = stickers[Cube::Face::U][y][x];
            sticker->setPolygon(poly);

            QColor colour = colours[cube->sticker(Cube::U, x, y)];

            sticker->setBrush(QBrush(colour));
            sticker->setPen(QPen(Qt::black, 0));
        }
    }

    //F face
    for(int y=0; y<s; y++){
        for(int x=0; x<s; x++){
            //the bottom left corner of the sticker on the cube [0,1]^3
            QVector3D v(x*delta, 0, (s-1-y)*delta);

            //scale the point to be in [-1,1]^3 instead of [0,1]^3
            v = 2 * v - QVector3D(1, 1, 1);

            //compute the isometric projection of the corner of the sticker
            //and scale it up so the cube has the required edge length
            QPointF point = proj.project(edgeLength/2 * v);

            QPolygonF poly;
            poly << point
                 << point + stickerSize * (basisImage[0])
                 << point + stickerSize * (basisImage[0] + basisImage[2])
                 << point + stickerSize * (basisImage[2])
                 << point;

            QGraphicsPolygonItem *sticker = stickers[Cube::Face::F][y][x];
            sticker->setPolygon(poly);

            QColor colour = colours[cube->sticker(Cube::F, x, y)];

            sticker->setBrush(QBrush(colour));
            sticker->setPen(QPen(Qt::black, 0));
        }
    }

    //R face
    for(int y=0; y<s; y++){
        for(int x=0; x<s; x++){
            //the bottom left corner of the sticker on the cube [0,1]^3
            QVector3D v(1, x*delta, (s-1-y)*delta);

            //scale the point to be in [-1,1]^3 instead of [0,1]^3
            v = 2 * v - QVector3D(1, 1, 1);

            //compute the isometric projection of the corner of the sticker
            //and scale it up so the cube has the required edge length
            QPointF point = edgeLength/2 * proj.project(v);

            QPolygonF poly;
            poly << point
                 << point + stickerSize * (basisImage[1])
                 << point + stickerSize * (basisImage[1] + basisImage[2])
                 << point + stickerSize * (basisImage[2])
                 << point;

            QGraphicsPolygonItem *sticker = stickers[Cube::Face::R][y][x];
            sticker->setPolygon(poly);

            QColor colour = colours[cube->sticker(Cube::R, x, y)];

            sticker->setBrush(QBrush(colour));
            sticker->setPen(QPen(Qt::black, 0));
        }
    }
}

void CubeGraphicsObject::setEdgeLength(qreal edgeLength){
    this->edgeLength = edgeLength;
}

void CubeGraphicsObject::setGapSize(qreal gapSize){
    this->gapSize = gapSize;
}

void CubeGraphicsObject::setProjection(float *mat){
    proj = Projection(QMatrix3x2(mat));
    reset();

    emit projectionChanged();
}

QJsonObject CubeGraphicsObject::toJSON(){
    QJsonObject data;

    data["edgeLength"] = edgeLength;
    data["gapSize"] = gapSize;
    data["projection"] = proj.toJSON();

    return data;
}

void CubeGraphicsObject::fromJSON(QJsonObject data){
    edgeLength = data["edgeLength"].toInt();
    gapSize = data["gapSize"].toInt();
    proj.fromJSON(data["projection"].toObject());

    updateAll();
}

void CubeGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event){
    lastMousePress = event->pos();
    event->accept();
}

void CubeGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    lastMouseRelease = event->pos();
    event->accept();

    //scale the point back to project([-1,1]^3), and unproject it
    bool ok;
    QVector3D vPressed = proj.unproject(lastMousePress*2/edgeLength, &ok);
    if(!ok) return;

    //scale it to be in [0,1]^3
    vPressed = (vPressed + QVector3D(1, 1, 1))/2;

    //which face was clicked
    Cube::Face clickedFace;

    //points on the face [0,1]^2 where the mouse was pressed/released
    QPointF facePosPress;
    QPointF facePosRelease;

    //we want to take the plane of the face where the mouse was pressed
    //and compute the position where the mouse was released on the plane
    //so that we can compute the angle in that plane to determine which
    //layer to move and in which direction
    QVector3D vReleased;
    if(vPressed.z() == 1){ //U face
        clickedFace = Cube::Face::U;
        facePosPress.setX(vPressed.x());
        facePosPress.setY(vPressed.y());

        vReleased = proj.unprojectU(lastMouseRelease*2/edgeLength);
        vReleased = (vReleased + QVector3D(1, 1, 1))/2;
        facePosRelease.setX(vReleased.x());
        facePosRelease.setY(vReleased.y());
    }
    else if(vPressed.y() == 0){ //F face
        clickedFace = Cube::Face::F;
        facePosPress.setX(vPressed.x());
        facePosPress.setY(vPressed.z());

        vReleased = proj.unprojectF(lastMouseRelease*2/edgeLength);
        vReleased = (vReleased + QVector3D(1, 1, 1))/2;
        facePosRelease.setX(vReleased.x());
        facePosRelease.setY(vReleased.z());
    }
    else if(vPressed.x() == 1){ //R face
        clickedFace = Cube::Face::R;
        facePosPress.setX(vPressed.y());
        facePosPress.setY(vPressed.z());

        vReleased = proj.unprojectR(lastMouseRelease*2/edgeLength);
        vReleased = (vReleased + QVector3D(1, 1, 1))/2;
        facePosRelease.setX(vReleased.y());
        facePosRelease.setY(vReleased.z());
    }

    //compute the angle
    qreal angle = atan2(facePosRelease.y() - facePosPress.y(),
                        facePosRelease.x() - facePosPress.x());

    Direction dragDirection;
    if(-pi/4 < angle && angle <= pi/4) dragDirection = Direction::Right;
    else if(pi/4 < angle && angle <= 3*pi/4) dragDirection = Direction::Up;
    else if(3*pi/4 < angle || angle <= -3*pi/4) dragDirection = Direction::Left;
    else if(-3*pi/4 < angle && angle <= -pi/4) dragDirection = Direction::Down;

    //compute the sticker that was clicked
    int s = cube->getSize();
    float delta = 1.0f/s + gapSize/(edgeLength*s);

    //which sticker was clicked on the face
    QPoint stickerPos(floor(facePosPress.x()/delta), s-1-floor(facePosPress.y()/delta));

    //rarely, we click right on the edge of a face, due to floating point
    //errors, the stickerPos coordinates can be -1 (or potentially s although
    //I haven't actually seen that happen yet), so we need to deal with that
    if(stickerPos.x() < 0) stickerPos.setX(0);
    if(stickerPos.y() < 0) stickerPos.setY(0);
    if(stickerPos.x() > s-1) stickerPos.setX(s-1);
    if(stickerPos.y() > s-1) stickerPos.setY(s-1);

    //determine which layer was moved and whether it was cw, ccw, or double
    Cube::Axis axis;
    int layer;
    bool clockwise; //true if clockwise wrt U,F,R faces

    if(clickedFace == Cube::Face::U){
        if(dragDirection == Direction::Right){
            axis = Cube::Axis::Z;
            layer = s-1-stickerPos.y();
            clockwise = true;
        }
        else if(dragDirection == Direction::Up){
            axis = Cube::Axis::X;
            layer = s-1-stickerPos.x();
            clockwise = true;
        }
        else if(dragDirection == Direction::Left){
            axis = Cube::Axis::Z;
            layer = s-1-stickerPos.y();
            clockwise = false;
        }
        else if(dragDirection == Direction::Down){
            axis = Cube::Axis::X;
            layer = s-1-stickerPos.x();
            clockwise = false;
        }
    }
    else if(clickedFace == Cube::Face::F){
        if(dragDirection == Direction::Right){
            axis = Cube::Axis::Y;
            layer = stickerPos.y();
            clockwise = false;
        }
        else if(dragDirection == Direction::Up){
            axis = Cube::Axis::X;
            layer = s-1-stickerPos.x();
            clockwise = true;
        }
        else if(dragDirection == Direction::Left){
            axis = Cube::Axis::Y;
            layer = stickerPos.y();
            clockwise = true;
        }
        else if(dragDirection == Direction::Down){
            axis = Cube::Axis::X;
            layer = s-1-stickerPos.x();
            clockwise = false;
        }
    }
    else if(clickedFace == Cube::Face::R){
        if(dragDirection == Direction::Right){
            axis = Cube::Axis::Y;
            layer = stickerPos.y();
            clockwise = false;
        }
        else if(dragDirection == Direction::Up){
            axis = Cube::Axis::Z;
            layer = stickerPos.x();
            clockwise = false;
        }
        else if(dragDirection == Direction::Left){
            axis = Cube::Axis::Y;
            layer = stickerPos.y();
            clockwise = true;
        }
        else if(dragDirection == Direction::Down){
            axis = Cube::Axis::Z;
            layer = stickerPos.x();
            clockwise = true;
        }
    }

    emit moveDrag(axis, layer, clockwise);
}

void CubeGraphicsObject::onMoveDone(Cube::Axis axis, int layerStart, int layerEnd, int){
    for(int i=layerStart; i<=layerEnd; i++){
        updateLayer(axis, i);
    }
}

void CubeGraphicsObject::updateSticker(Cube::Face face, int x, int y){
    int piece = cube->sticker(face, x, y);
    QColor colour = colours[piece];
    QGraphicsPolygonItem *sticker = stickers[face][y][x];

    sticker->setBrush(QBrush(colour));
    sticker->update();
}

void CubeGraphicsObject::updateFace(Cube::Face face){
    int s = cube->getSize();
    for(int y=0; y<s; y++){
        for(int x=0; x<s; x++){
            updateSticker(face, x, y);
        }
    }
}

void CubeGraphicsObject::updateLayer(Cube::Axis axis, int layer){
    int s = cube->getSize();

    if(axis == Cube::Axis::X){ //R-L moves
        for(int i=0; i<s; i++){
            updateSticker(Cube::Face::U, s-1-layer, i);
            updateSticker(Cube::Face::F, s-1-layer, i);
        }

        if(layer == 0) updateFace(Cube::Face::R);
    }
    else if(axis == Cube::Axis::Y){ //U-D moves
        for(int i=0; i<s; i++){
            updateSticker(Cube::Face::F, i, layer);
            updateSticker(Cube::Face::R, i, layer);
        }

        if(layer == 0) updateFace(Cube::Face::U);
    }
    else if(axis == Cube::Axis::Z){ //F-B moves
        for(int i=0; i<s; i++){
            updateSticker(Cube::Face::U, i, s-1-layer);
            updateSticker(Cube::Face::R, layer, i);
        }

        if(layer == 0) updateFace(Cube::Face::F);
    }
}

void CubeGraphicsObject::updateAll(){
    int s = cube->getSize();
    for(int face=0; face<3; face++){
        for(int y=0; y<s; y++){
            for(int x=0; x<s; x++){
                int piece = cube->sticker((Cube::Face)face, x, y);
                QGraphicsPolygonItem *sticker = stickers[face][y][x];
                if(sticker->brush().color() != colours[piece]){
                    sticker->setBrush(QBrush(colours[piece]));
                }
            }
        }
    }
}
