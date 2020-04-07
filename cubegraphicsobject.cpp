#include "cubegraphicsobject.h"

CubeGraphicsObject::CubeGraphicsObject(Cube *c, Settings *s, QGraphicsObject *parent) :
    QGraphicsObject(parent)
{
    cube = c;
    settings = s;

    connect(cube, SIGNAL(moveDone(Move)), this, SLOT(onMoveDone(Move)));
    connect(cube, SIGNAL(rotationDone(Move)), this, SLOT(onRotationDone()));
    connect(cube, SIGNAL(cubeReset()), this, SLOT(onCubeReset()));
    connect(cube, SIGNAL(cubeScrambled()), this, SLOT(onCubeScrambled()));
    connect(cube, SIGNAL(cubeStateChanged()), this, SLOT(onCubeStateChanged()));
    connect(cube, SIGNAL(cubeSizeChanged()), this, SLOT(onCubeSizeChanged()));

    connect(settings, SIGNAL(lineColourChanged()), this, SLOT(onLineColourSettingChanged()));
    connect(settings, SIGNAL(lineWidthChanged()), this, SLOT(onLineWidthSettingChanged()));
    connect(settings, SIGNAL(coloursChanged()), this, SLOT(onColoursSettingChanged()));
    connect(settings, SIGNAL(guideLinesCrossChanged()), this, SLOT(onGuideLinesCrossSettingChanged()));
    connect(settings, SIGNAL(guideLinesPlusChanged()), this, SLOT(onGuideLinesPlusSettingChanged()));
    connect(settings, SIGNAL(guideLinesBoxChanged()), this, SLOT(onGuideLinesBoxSettingChanged()));
    connect(settings, SIGNAL(guideLineColourChanged()), this, SLOT(onGuideLineColourSettingChanged()));
    connect(settings, SIGNAL(guideLineWidthChanged()), this, SLOT(onGuideLineWidthSettingChanged()));
    connect(settings, SIGNAL(supercubeChanged()), this, SLOT(onSupercubeSettingChanged()));
    connect(settings, SIGNAL(supercubeStickersChanged()), this, SLOT(onSupercubeStickersSettingChanged()));
    connect(settings, SIGNAL(pochmannBarThicknessChanged()), this, SLOT(onPochmannBarThicknessSettingChanged()));
    connect(settings, SIGNAL(pochmannCageChanged()), this, SLOT(onPochmannCageSettingChanged()));

    float mat[6] = {1/sqrt(2), 1/sqrt(2), 0, -1/sqrt(6), 1/sqrt(6), sqrt(2./3)};
    proj = Projection(QMatrix3x2(mat));

    setEdgeLength(350);
    setGapSize(0);

    reset();
}

QRectF CubeGraphicsObject::boundingRect() const{
    return childrenBoundingRect();
}

void CubeGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    int cubeSize = cube->getSize();

    //scale the visible rect so that the cube (in 3d space) is [-1,1]^3
    QPolygonF r = visibleRect * QTransform::fromScale(2/edgeLength, 2/edgeLength);

    qDebug() << "visible rect:" << visibleRect;

    QPolygonF polyU = proj.unprojectU(r)
                          .boundingRect()
                          .intersected(QRectF(-1,-1,2,2))
                          * QTransform::fromTranslate(1, 1)
                          * QTransform::fromScale(0.5, 0.5)
                          * QTransform::fromScale(cubeSize, cubeSize);
    QPolygonF polyF = proj.unprojectF(r)
                          .boundingRect()
                          .intersected(QRectF(-1,-1,2,2))
                          * QTransform::fromTranslate(1, 1)
                          * QTransform::fromScale(0.5, 0.5)
                          * QTransform::fromScale(cubeSize, cubeSize);
    QPolygonF polyR = proj.unprojectR(r)
                          .boundingRect()
                          .intersected(QRectF(-1,-1,2,2))
                          * QTransform::fromTranslate(1, 1)
                          * QTransform::fromScale(0.5, 0.5)
                          * QTransform::fromScale(cubeSize, cubeSize);

    qDebug() << polyU << polyF << polyR;

    QRectF rectU = polyU.boundingRect();
    QRectF rectF = polyF.boundingRect();
    QRectF rectR = polyR.boundingRect();

    qDebug() << rectU << rectF << rectR;

#define F(x) (qMax(qMin((int)floor(x), cubeSize-1), 0))

    int startUx = F(rectU.left());
    int   endUx = F(rectU.right());
    int startUy = F(rectU.top());
    int   endUy = F(rectU.bottom());
    int startFx = F(rectF.left());
    int   endFx = F(rectF.right());
    int startFy = F(rectF.top());
    int   endFy = F(rectF.bottom());
    int startRx = F(rectR.left());
    int   endRx = F(rectR.right());
    int startRy = F(rectR.top());
    int   endRy = F(rectR.bottom());

#undef F

    qDebug() << "U:" << startUx << "to" << endUx << "and" << startUy << "to" << endUy;
    qDebug() << "F:" << startFx << "to" << endFx << "and" << startFy << "to" << endFy;
    qDebug() << "R:" << startRx << "to" << endRx << "and" << startRy << "to" << endRy;

    int numU = (endUx+1-startUx)*(endUy+1-startUy);
    int numF = (endFx+1-startFx)*(endFy+1-startFy);
    int numR = (endRx+1-startRx)*(endRy+1-startRy);

    qDebug() << "number of stickers to paint:" << numU << numF << numR;

    QTransform t = painter->transform();
    for(int y=startUy; y<=endUy; y++){
        for(int x=startUx; x<=endUx; x++){
            Sticker *s = stickers[Face::U][cubeSize-1-y][x];
            painter->setTransform(t);
            painter->translate(s->pos());
            painter->setTransform(s->transform(), true);
            s->paint(painter, option, widget);
        }
    }

    for(int y=startFy; y<=endFy; y++){
        for(int x=startFx; x<=endFx; x++){
            Sticker *s = stickers[Face::F][cubeSize-1-y][x];
            painter->setTransform(t);
            painter->translate(s->pos());
            painter->setTransform(s->transform(), true);
            s->paint(painter, option, widget);
        }
    }

    for(int y=startRy; y<=endRy; y++){
        for(int x=startRx; x<=endRx; x++){
            Sticker *s = stickers[Face::R][cubeSize-1-y][x];
            painter->setTransform(t);
            painter->translate(s->pos());
            painter->setTransform(s->transform(), true);
            s->paint(painter, option, widget);
        }
    }
}

void CubeGraphicsObject::setEdgeLength(qreal edgeLength){
    this->edgeLength = edgeLength;
}

void CubeGraphicsObject::setGapSize(qreal gapSize){
    this->gapSize = gapSize;
}

Projection CubeGraphicsObject::getProjection(){
    return proj;
}

void CubeGraphicsObject::setProjection(float *mat){
    proj = Projection(QMatrix3x2(mat));
    reset();

    emit projectionChanged();
}

Sticker *CubeGraphicsObject::getSticker(Face f, int x, int y){
    assert(0 <= (int)f && (int)f <= 6);
    assert(0 <= x && x < cube->getSize());
    assert(0 <= y && y < cube->getSize());

    return stickers[f][y][x];
}

void CubeGraphicsObject::setVisibleRect(QRectF r){
    visibleRect = r;
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

    reset();
}

void CubeGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event){
    lastMousePress = event->pos();
    event->accept();
}

void CubeGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QElapsedTimer t;
    t.start();

    lastMouseRelease = event->pos();
    event->accept();

    //scale the point back to project([-1,1]^3), and unproject it
    bool ok;
    QVector3D vPressed = proj.unproject(lastMousePress*2/edgeLength, &ok);
    if(!ok) return;

    //scale it to be in [0,1]^3
    vPressed = (vPressed + QVector3D(1, 1, 1))/2;

    //which face was clicked
    Face clickedFace;

    //points on the face [0,1]^2 where the mouse was pressed/released
    QPointF facePosPress;
    QPointF facePosRelease;

    //we want to take the plane of the face where the mouse was pressed
    //and compute the position where the mouse was released on the plane
    //so that we can compute the angle in that plane to determine which
    //layer to move and in which direction
    QVector3D vReleased;
    if(vPressed.z() == 1){ //U face
        clickedFace = Face::U;
        facePosPress.setX(vPressed.x());
        facePosPress.setY(vPressed.y());

        vReleased = proj.unprojectU(lastMouseRelease*2/edgeLength);
        vReleased = (vReleased + QVector3D(1, 1, 1))/2;
        facePosRelease.setX(vReleased.x());
        facePosRelease.setY(vReleased.y());
    }
    else if(vPressed.y() == 0){ //F face
        clickedFace = Face::F;
        facePosPress.setX(vPressed.x());
        facePosPress.setY(vPressed.z());

        vReleased = proj.unprojectF(lastMouseRelease*2/edgeLength);
        vReleased = (vReleased + QVector3D(1, 1, 1))/2;
        facePosRelease.setX(vReleased.x());
        facePosRelease.setY(vReleased.z());
    }
    else if(vPressed.x() == 1){ //R face
        clickedFace = Face::R;
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
    Axis axis;
    int layer;
    bool clockwise; //true if clockwise wrt U,F,R faces

    if(clickedFace == Face::U){
        if(dragDirection == Direction::Right){
            axis = Axis::Z;
            layer = s-1-stickerPos.y();
            clockwise = true;
        }
        else if(dragDirection == Direction::Up){
            axis = Axis::X;
            layer = s-1-stickerPos.x();
            clockwise = true;
        }
        else if(dragDirection == Direction::Left){
            axis = Axis::Z;
            layer = s-1-stickerPos.y();
            clockwise = false;
        }
        else if(dragDirection == Direction::Down){
            axis = Axis::X;
            layer = s-1-stickerPos.x();
            clockwise = false;
        }
    }
    else if(clickedFace == Face::F){
        if(dragDirection == Direction::Right){
            axis = Axis::Y;
            layer = stickerPos.y();
            clockwise = false;
        }
        else if(dragDirection == Direction::Up){
            axis = Axis::X;
            layer = s-1-stickerPos.x();
            clockwise = true;
        }
        else if(dragDirection == Direction::Left){
            axis = Axis::Y;
            layer = stickerPos.y();
            clockwise = true;
        }
        else if(dragDirection == Direction::Down){
            axis = Axis::X;
            layer = s-1-stickerPos.x();
            clockwise = false;
        }
    }
    else if(clickedFace == Face::R){
        if(dragDirection == Direction::Right){
            axis = Axis::Y;
            layer = stickerPos.y();
            clockwise = false;
        }
        else if(dragDirection == Direction::Up){
            axis = Axis::Z;
            layer = stickerPos.x();
            clockwise = false;
        }
        else if(dragDirection == Direction::Left){
            axis = Axis::Y;
            layer = stickerPos.y();
            clockwise = true;
        }
        else if(dragDirection == Direction::Down){
            axis = Axis::Z;
            layer = stickerPos.x();
            clockwise = true;
        }
    }

    qDebug() << "Detected move drag in" << t.elapsed() << "ms";

    emit moveDrag(axis, layer, clockwise, event->button());
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

    //some constants
    int s = cube->getSize();
    //float stickerSize = (edgeLength - (s-1) * gapSize)/s;
    //float delta = ((edgeLength - (s-1) * gapSize)/s + gapSize)/edgeLength;
    //simplified above formulas to reduce floating point errors
    float stickerSize = (edgeLength+gapSize)/s - gapSize;
    float delta = 1.0f/s + gapSize/(edgeLength*s);

    for(int face=0; face<3; face++){
        stickers.append(QList<QList<Sticker*>>());
        for(int y=0; y<cube->getSize(); y++){
            stickers[face].append(QList<Sticker*>());
            for(int x=0; x<cube->getSize(); x++){
                //todo: make proj a pointer
                Sticker *sticker = new Sticker((Face)face, QPoint(x, y), cube, settings, &proj, stickerSize);
                stickers[face][y].append(sticker);
            }
        }
    }

    //move the stickers into place
    for(int y=0; y<s; y++){
        for(int x=0; x<s; x++){
            Sticker *sticker;

            //the coordinates of the bottom left corner of the sticker
            //in 3d space on the cube [0,1]^3 (then scaled to [-1,1]^3)
            QVector3D v;

            //the projected coordinates
            QPointF point;

            sticker = stickers[Face::U][y][x];
            v = QVector3D(x*delta, (s-1-y)*delta, 1);
            v = 2 * v - QVector3D(1, 1, 1);
            point = edgeLength/2 * proj.project(v);
            sticker->setPos(point);

            sticker = stickers[Face::F][y][x];
            v = QVector3D(x*delta, 0, (s-1-y)*delta);
            v = 2 * v - QVector3D(1, 1, 1);
            point = edgeLength/2 * proj.project(v);
            sticker->setPos(point);

            sticker = stickers[Face::R][y][x];
            v = QVector3D(1, x*delta, (s-1-y)*delta);
            v = 2 * v - QVector3D(1, 1, 1);
            point = edgeLength/2 * proj.project(v);
            sticker->setPos(point);
        }
    }

    //set the brush and pen of each sticker
    for(int face=0; face<3; face++){
        for(int y=0; y<s; y++){
            for(int x=0; x<s; x++){
                Sticker *sticker = stickers[face][y][x];

                QColor colour = settings->getColour((Face)cube->sticker((Face)face, x, y));

                sticker->setBrush(QBrush(colour));
                sticker->setPen(QPen(settings->getLineColour(), settings->getLineWidth()));
            }
        }
    }

    //guide lines
    QGraphicsLineItem *line;
    QPointF p1, p2;

    //X
    for(int i=0; i<guideLinesCross.size(); i++){
        delete guideLinesCross[i];
    }

    guideLinesCross.clear();

    //U face
    line = new QGraphicsLineItem(this);
    p1 = edgeLength/2 * proj.project(QVector3D(-1, -1, 1));
    p2 = edgeLength/2 * proj.project(QVector3D(1, 1, 1));
    line->setLine(QLineF(p1, p2));
    guideLinesCross.append(line);

    line = new QGraphicsLineItem(this);
    p1 = edgeLength/2 * proj.project(QVector3D(-1, 1, 1));
    p2 = edgeLength/2 * proj.project(QVector3D(1, -1, 1));
    line->setLine(QLineF(p1, p2));
    guideLinesCross.append(line);

    //F face
    line = new QGraphicsLineItem(this);
    p1 = edgeLength/2 * proj.project(QVector3D(-1, -1, -1));
    p2 = edgeLength/2 * proj.project(QVector3D(1, -1, 1));
    line->setLine(QLineF(p1, p2));
    guideLinesCross.append(line);

    line = new QGraphicsLineItem(this);
    p1 = edgeLength/2 * proj.project(QVector3D(-1, -1, 1));
    p2 = edgeLength/2 * proj.project(QVector3D(1, -1, -1));
    line->setLine(QLineF(p1, p2));
    guideLinesCross.append(line);

    //R face
    line = new QGraphicsLineItem(this);
    p1 = edgeLength/2 * proj.project(QVector3D(1, -1, -1));
    p2 = edgeLength/2 * proj.project(QVector3D(1, 1, 1));
    line->setLine(QLineF(p1, p2));
    guideLinesCross.append(line);

    line = new QGraphicsLineItem(this);
    p1 = edgeLength/2 * proj.project(QVector3D(1, -1, 1));
    p2 = edgeLength/2 * proj.project(QVector3D(1, 1, -1));
    line->setLine(QLineF(p1, p2));
    guideLinesCross.append(line);

    foreach(QGraphicsLineItem *l, guideLinesCross){
        //set the colour of the guide lines
        l->setPen(QPen(settings->getGuideLineColour(), settings->getGuideLineWidth()));

        //make the guide lines invisible if they are disabled in settings
        l->setVisible(settings->getGuideLinesCross());
    }

    //+
    for(int i=0; i<guideLinesPlus.size(); i++){
        delete guideLinesPlus[i];
    }

    guideLinesPlus.clear();

    //U face
    line = new QGraphicsLineItem(this);
    p1 = edgeLength/2 * proj.project(QVector3D(0, -1, 1));
    p2 = edgeLength/2 * proj.project(QVector3D(0, 1, 1));
    line->setLine(QLineF(p1, p2));
    guideLinesPlus.append(line);

    line = new QGraphicsLineItem(this);
    p1 = edgeLength/2 * proj.project(QVector3D(-1, 0, 1));
    p2 = edgeLength/2 * proj.project(QVector3D(1, 0, 1));
    line->setLine(QLineF(p1, p2));
    guideLinesPlus.append(line);

    //F face
    line = new QGraphicsLineItem(this);
    p1 = edgeLength/2 * proj.project(QVector3D(0, -1, -1));
    p2 = edgeLength/2 * proj.project(QVector3D(0, -1, 1));
    line->setLine(QLineF(p1, p2));
    guideLinesPlus.append(line);

    line = new QGraphicsLineItem(this);
    p1 = edgeLength/2 * proj.project(QVector3D(-1, -1, 0));
    p2 = edgeLength/2 * proj.project(QVector3D(1, -1, 0));
    line->setLine(QLineF(p1, p2));
    guideLinesPlus.append(line);

    //R face
    line = new QGraphicsLineItem(this);
    p1 = edgeLength/2 * proj.project(QVector3D(1, 0, -1));
    p2 = edgeLength/2 * proj.project(QVector3D(1, 0, 1));
    line->setLine(QLineF(p1, p2));
    guideLinesPlus.append(line);

    line = new QGraphicsLineItem(this);
    p1 = edgeLength/2 * proj.project(QVector3D(1, -1, 0));
    p2 = edgeLength/2 * proj.project(QVector3D(1, 1, 0));
    line->setLine(QLineF(p1, p2));
    guideLinesPlus.append(line);

    foreach(QGraphicsLineItem *l, guideLinesPlus){
        //set the colour of the guide lines
        l->setPen(QPen(settings->getGuideLineColour(), settings->getGuideLineWidth()));

        //make the guide lines invisible if they are disabled in settings
        l->setVisible(settings->getGuideLinesPlus());
    }

    //box
    for(int i=0; i<guideLinesBox.size(); i++){
        delete guideLinesBox[i];
    }

    guideLinesBox.clear();

    //if odd cube, draw a box around the center sticker
    if(s%2 == 1){
        for(int face=0; face<3; face++){
            QGraphicsPolygonItem *b = new QGraphicsPolygonItem(this);

            Sticker *centerSticker = stickers[face][s/2][s/2];
            b->setPolygon(QGraphicsItem::mapFromItem(centerSticker, centerSticker->rect()));

            guideLinesBox.append(b);
        }
    }
    //if even cube except 2x2, draw a box around the center 4 stickers
    else if(s != 2){
        for(int face=0; face<3; face++){
            QGraphicsPolygonItem *b = new QGraphicsPolygonItem(this);

            //the polygon we want is formed of:
            //the bottom left corner of stickers[face][s/2][s/2-1]
            //the top left corner of stickers[face][s/2-1][s/2-1]
            //the top right corner of stickers[face][s/2-1][s/2]
            //the bottom right corner of stickers[face][s/2][s/2]

            QPolygonF poly;
            poly << stickers[face][s/2  ][s/2-1]->mapToParent(stickers[face][s/2  ][s/2-1]->rect().bottomLeft())
                 << stickers[face][s/2-1][s/2-1]->mapToParent(stickers[face][s/2-1][s/2-1]->rect().topLeft())
                 << stickers[face][s/2-1][s/2  ]->mapToParent(stickers[face][s/2-1][s/2  ]->rect().topRight())
                 << stickers[face][s/2  ][s/2  ]->mapToParent(stickers[face][s/2  ][s/2  ]->rect().bottomRight());

            b->setPolygon(poly);

            guideLinesBox.append(b);
        }
    }

    foreach(QGraphicsPolygonItem *b, guideLinesBox){
        //set the colour of the guide lines
        b->setPen(QPen(settings->getGuideLineColour(), settings->getGuideLineWidth()));

        //set the brush to be transparent (not filled in)
        b->setBrush(QBrush(Qt::transparent));

        //make the guide lines invisible if they are disabled in settings
        b->setVisible(settings->getGuideLinesBox());
    }
}

void CubeGraphicsObject::updateSticker(Face face, int x, int y){
    int piece = cube->sticker(face, x, y);

    QColor colour = settings->getColour((Face)piece);
    Sticker *sticker = stickers[face][y][x];

    sticker->setBrush(QBrush(colour));
    sticker->update();
}

void CubeGraphicsObject::updateFace(Face face){
    int s = cube->getSize();
    for(int y=0; y<s; y++){
        for(int x=0; x<s; x++){
            updateSticker(face, x, y);
        }
    }
}

void CubeGraphicsObject::updateLayer(Axis axis, int layer){
    int s = cube->getSize();

    if(axis == Axis::X){ //R-L moves
        for(int i=0; i<s; i++){
            updateSticker(Face::U, s-1-layer, i);
            updateSticker(Face::F, s-1-layer, i);
        }

        if(layer == 0) updateFace(Face::R);
    }
    else if(axis == Axis::Y){ //U-D moves
        for(int i=0; i<s; i++){
            updateSticker(Face::F, i, layer);
            updateSticker(Face::R, i, layer);
        }

        if(layer == 0) updateFace(Face::U);
    }
    else if(axis == Axis::Z){ //F-B moves
        for(int i=0; i<s; i++){
            updateSticker(Face::U, i, s-1-layer);
            updateSticker(Face::R, layer, i);
        }

        if(layer == 0) updateFace(Face::F);
    }
}

void CubeGraphicsObject::updateAll(){
    int s = cube->getSize();
    for(int face=0; face<3; face++){
        for(int y=0; y<s; y++){
            for(int x=0; x<s; x++){
                updateSticker((Face)face, x, y);
            }
        }
    }

}

void CubeGraphicsObject::onMoveDone(Move move){
    QElapsedTimer t;
    t.start();

    Axis axis = move.getAxis();
    int layerStart = move.getLayerStart();
    int layerEnd = move.getLayerEnd();

    for(int i=layerStart; i<=layerEnd; i++){
        updateLayer(axis, i);
    }

    qDebug() << "Scheduled sticker updates in" << t.elapsed() << "ms";
}

void CubeGraphicsObject::onRotationDone(){
    updateAll();
}

void CubeGraphicsObject::onCubeReset(){
    updateAll();
}

void CubeGraphicsObject::onCubeScrambled(){
    updateAll();
}

void CubeGraphicsObject::onCubeStateChanged(){
    updateAll();
}

void CubeGraphicsObject::onCubeSizeChanged(){
    reset();
}

void CubeGraphicsObject::onLineColourSettingChanged(){
    int s = cube->getSize();
    for(int face=0; face<3; face++){
        for(int y=0; y<s; y++){
            for(int x=0; x<s; x++){
                Sticker *sticker = stickers[face][y][x];
                QPen pen = sticker->pen();
                pen.setColor(settings->getLineColour());
                stickers[face][y][x]->setPen(pen);
            }
        }
    }
}

void CubeGraphicsObject::onLineWidthSettingChanged(){
    int s = cube->getSize();
    for(int face=0; face<3; face++){
        for(int y=0; y<s; y++){
            for(int x=0; x<s; x++){
                Sticker *sticker = stickers[face][y][x];
                QPen pen = sticker->pen();
                pen.setWidth(settings->getLineWidth());
                stickers[face][y][x]->setPen(pen);
            }
        }
    }
}

void CubeGraphicsObject::onColoursSettingChanged(){
    updateAll();
}

void CubeGraphicsObject::onGuideLinesCrossSettingChanged(){
    foreach(QGraphicsLineItem *l, guideLinesCross){
        l->setVisible(settings->getGuideLinesCross());
    }
}

void CubeGraphicsObject::onGuideLinesPlusSettingChanged(){
    foreach(QGraphicsLineItem *l, guideLinesPlus){
        l->setVisible(settings->getGuideLinesPlus());
    }
}

void CubeGraphicsObject::onGuideLinesBoxSettingChanged(){
    foreach(QGraphicsPolygonItem *b, guideLinesBox){
        b->setVisible(settings->getGuideLinesBox());
    }
}

void CubeGraphicsObject::onGuideLineColourSettingChanged(){
    foreach(QGraphicsLineItem *l, guideLinesCross){
        QPen pen = l->pen();
        pen.setColor(settings->getGuideLineColour());
        l->setPen(pen);
    }
    foreach(QGraphicsLineItem *l, guideLinesPlus){
        QPen pen = l->pen();
        pen.setColor(settings->getGuideLineColour());
        l->setPen(pen);
    }
    foreach(QGraphicsPolygonItem *b, guideLinesBox){
        QPen pen = b->pen();
        pen.setColor(settings->getGuideLineColour());
        b->setPen(pen);
    }
}

void CubeGraphicsObject::onGuideLineWidthSettingChanged(){
    foreach(QGraphicsLineItem *l, guideLinesCross){
        QPen pen = l->pen();
        pen.setWidth(settings->getGuideLineWidth());
        l->setPen(pen);
    }
    foreach(QGraphicsLineItem *l, guideLinesPlus){
        QPen pen = l->pen();
        pen.setWidth(settings->getGuideLineWidth());
        l->setPen(pen);
    }
    foreach(QGraphicsPolygonItem *b, guideLinesBox){
        QPen pen = b->pen();
        pen.setWidth(settings->getGuideLineWidth());
        b->setPen(pen);
    }
}

void CubeGraphicsObject::onSupercubeSettingChanged(){
    updateAll();
}

void CubeGraphicsObject::onSupercubeStickersSettingChanged(){
    updateAll();
}

void CubeGraphicsObject::onPochmannBarThicknessSettingChanged(){
    updateAll();
}

void CubeGraphicsObject::onPochmannCageSettingChanged(){
    updateAll();
}
