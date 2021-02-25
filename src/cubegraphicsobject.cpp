#include "cubegraphicsobject.h"

CubeGraphicsObject::CubeGraphicsObject(Cube *c, Settings *s, QGraphicsObject *parent) :
    QGraphicsObject(parent)
{
    cube = c;
    settings = s;

    connect(cube, SIGNAL(moveDone(Move)), this, SLOT(onMoveDone()));
    connect(cube, SIGNAL(rotationDone(Move)), this, SLOT(onRotationDone()));
    connect(cube, SIGNAL(cubeReset()), this, SLOT(onCubeReset()));
    connect(cube, SIGNAL(cubeScrambled()), this, SLOT(onCubeScrambled()));
    connect(cube, SIGNAL(cubeStateChanged()), this, SLOT(onCubeStateChanged()));
    connect(cube, SIGNAL(cubeSizeChanged()), this, SLOT(onCubeSizeChanged()));

    connect(settings, SIGNAL(lineColourChanged()), this, SLOT(onLineColourSettingChanged()));
    connect(settings, SIGNAL(lineWidthChanged()), this, SLOT(onLineWidthSettingChanged()));
    connect(settings, SIGNAL(coloursChanged()), this, SLOT(onColoursSettingChanged()));
    connect(settings, SIGNAL(guideLinesCrossChanged()), this, SLOT(onGuideLinesSettingChanged()));
    connect(settings, SIGNAL(guideLinesPlusChanged()), this, SLOT(onGuideLinesSettingChanged()));
    connect(settings, SIGNAL(guideLinesBoxChanged()), this, SLOT(onGuideLinesSettingChanged()));
    connect(settings, SIGNAL(guideLineColourChanged()), this, SLOT(onGuideLinesSettingChanged()));
    connect(settings, SIGNAL(guideLineWidthChanged()), this, SLOT(onGuideLinesSettingChanged()));
    connect(settings, SIGNAL(supercubeChanged()), this, SLOT(onSupercubeSettingChanged()));
    connect(settings, SIGNAL(supercubeStickersChanged()), this, SLOT(onSupercubeStickersSettingChanged()));
    connect(settings, SIGNAL(pochmannBarThicknessChanged()), this, SLOT(onPochmannBarThicknessSettingChanged()));
    connect(settings, SIGNAL(pochmannCageChanged()), this, SLOT(onPochmannCageSettingChanged()));

    float mat[6] = {1/sqrt(2), 1/sqrt(2), 0, -1/sqrt(6), 1/sqrt(6), sqrt(2./3)};
    proj = Projection(QMatrix3x2(mat));

    setEdgeLength(350);
    setGapSize(0);

    drawGuideLines();
}

QRectF CubeGraphicsObject::boundingRect() const{
    return childrenBoundingRect();
}

void CubeGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    int s = cube->getSize();

    //scale the visible rect so that the cube (in 3d space) is [-1,1]^3
    QPolygonF r = visibleRect * QTransform::fromScale(2/edgeLength, 2/edgeLength);

    QPolygonF polyU = proj.unprojectU(r)
                          .boundingRect()
                          .intersected(QRectF(-1,-1,2,2))
                          * QTransform::fromTranslate(1, 1)
                          * QTransform::fromScale(0.5, 0.5)
                          * QTransform::fromScale(s, s);
    QPolygonF polyF = proj.unprojectF(r)
                          .boundingRect()
                          .intersected(QRectF(-1,-1,2,2))
                          * QTransform::fromTranslate(1, 1)
                          * QTransform::fromScale(0.5, 0.5)
                          * QTransform::fromScale(s, s);
    QPolygonF polyR = proj.unprojectR(r)
                          .boundingRect()
                          .intersected(QRectF(-1,-1,2,2))
                          * QTransform::fromTranslate(1, 1)
                          * QTransform::fromScale(0.5, 0.5)
                          * QTransform::fromScale(s, s);

    QRectF rectU = polyU.boundingRect();
    QRectF rectF = polyF.boundingRect();
    QRectF rectR = polyR.boundingRect();

#define F(x) (qMax(qMin((int)floor(x), s-1), 0))

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

    QTransform t = painter->transform();

    auto drawSticker = [&](Face f, int x, int y){
        float stickerSize = (edgeLength+gapSize)/s - gapSize;
        Sticker sticker(f, QPoint(x, s-1-y), cube, settings, &proj, stickerSize);

        //compute the position
        QVector3D v = getStickerPos(f, x, s-1-y);
        QPointF point = edgeLength/2 * proj.project(v);
        sticker.setPos(point);

        //set the stickers colour
        int piece = cube->sticker(f, x, s-1-y);
        QColor colour = settings->getColour((Face)piece);
        sticker.setBrush(QBrush(colour));
        sticker.setPen(QPen(settings->getLineColour(), settings->getLineWidth()));

        //draw the sticker
        painter->setTransform(t);
        painter->translate(sticker.pos());
        painter->setTransform(sticker.transform(), true);
        sticker.paint(painter, option, widget);
    };

    for(int y=startUy; y<=endUy; y++){
        for(int x=startUx; x<=endUx; x++){
            drawSticker(Face::U, x, y);
        }
    }

    for(int y=startFy; y<=endFy; y++){
        for(int x=startFx; x<=endFx; x++){
            drawSticker(Face::F, x, y);
        }
    }

    for(int y=startRy; y<=endRy; y++){
        for(int x=startRx; x<=endRx; x++){
            drawSticker(Face::R, x, y);
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
    drawGuideLines();

    emit projectionChanged();
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
    emit projectionChanged();

    //redraw the guide lines
    drawGuideLines();
}

void CubeGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event){
    lastMousePress = event->pos();
}

void CubeGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    lastMouseRelease = event->pos();

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
    else{ //R face, vPressed.x() == 1
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
    if(-pi/4 < angle && angle <= pi/4){
        dragDirection = Direction::Right;
    }
    else if(pi/4 < angle && angle <= 3*pi/4){
        dragDirection = Direction::Up;
    }
    else if(3*pi/4 < angle || angle <= -3*pi/4){
        dragDirection = Direction::Left;
    }
    else{ //-3*pi/4 < angle && angle <= -pi/4
        dragDirection = Direction::Down;
    }

    //compute the sticker that was clicked
    int s = cube->getSize();
    float delta = 1.0f/s + gapSize/(edgeLength*s);

    //which sticker was clicked on the face
    QPoint stickerPos(floor(facePosPress.x()/delta), s-1-floor(facePosPress.y()/delta));

    //rarely, we click right on the edge of a face, due to floating point
    //errors, the stickerPos coordinates can be -1 (or potentially s although
    //I haven't actually seen that happen yet), so we need to deal with that
    if(stickerPos.x() <   0) stickerPos.setX(0);
    if(stickerPos.y() <   0) stickerPos.setY(0);
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

    emit moveDrag(axis, layer, clockwise, event->button());
}

void CubeGraphicsObject::drawGuideLines(){
    auto makeLine = [this](QPointF p1, QPointF p2, Face face, bool enabled){
        QPointF pt1 = edgeLength/2 * p1;
        QPointF pt2 = edgeLength/2 * p2;
        QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(pt1, pt2), this);
        line->setTransform(proj.toTransform(face));
        line->setPos(edgeLength/2 * proj.project(faceCenter(face)));

        QPen p;
        p.setColor(settings->getGuideLineColour());
        p.setWidth(settings->getGuideLineWidth());
        p.setCosmetic(true);
        p.setCapStyle(Qt::FlatCap);
        line->setPen(p);

        line->setVisible(enabled && proj.isFaceVisible(face));

        return line;
    };

    QList<Face> faces = {U, F, R};

    //X
    for(int i=0; i<guideLinesCross.size(); i++){
        delete guideLinesCross[i];
    }
    guideLinesCross.clear();

    foreach(Face f, faces){
        bool enabled = settings->getGuideLinesCross();
        guideLinesCross.append(makeLine(QPointF(-1, -1), QPointF(1, 1), f, enabled));
        guideLinesCross.append(makeLine(QPointF(-1, 1), QPointF(1, -1), f, enabled));
    }

    //+
    for(int i=0; i<guideLinesPlus.size(); i++){
        delete guideLinesPlus[i];
    }
    guideLinesPlus.clear();

    foreach(Face f, faces){
        bool enabled = settings->getGuideLinesPlus();
        guideLinesPlus.append(makeLine(QPointF(-1, 0), QPointF(1, 0), f, enabled));
        guideLinesPlus.append(makeLine(QPointF(0, -1), QPointF(0, 1), f, enabled));
    }

    //box
    for(int i=0; i<guideLinesBox.size(); i++){
        delete guideLinesBox[i];
    }
    guideLinesBox.clear();

    int s = cube->getSize();
    float stickerSize = (edgeLength+gapSize)/s - gapSize;

    foreach(Face f, faces){
        QRectF r;

        //if odd cube, draw a box around the center sticker
        if(s%2 == 1){
            r = QRectF(-stickerSize/2, -stickerSize/2, stickerSize, stickerSize);
        }
        //if even cube except 2x2, draw a box around the center 4 stickers
        else if(s != 2){
            r = QRectF(-stickerSize, -stickerSize, 2*stickerSize, 2*stickerSize);
        }

        QGraphicsRectItem *b = new QGraphicsRectItem(r, this);
        b->setTransform(proj.toTransform(f));
        b->setPos(edgeLength/2 * proj.project(faceCenter(f)));
        guideLinesBox.append(b);

        //set the pen
        QPen p;
        p.setColor(settings->getGuideLineColour());
        p.setWidth(settings->getGuideLineWidth());
        p.setCosmetic(true);
        p.setJoinStyle(Qt::MiterJoin);
        b->setPen(p);

        //set the brush to be transparent (not filled in)
        b->setBrush(QBrush(Qt::transparent));

        //make the guide lines invisible if they are disabled in settings
        b->setVisible(settings->getGuideLinesBox() && proj.isFaceVisible(f));
    }
}

QVector3D CubeGraphicsObject::getStickerPos(Face f, int x, int y){
    int s = cube->getSize();
    float delta = 1.0f/s + gapSize/(edgeLength*s);

    QVector3D v;

    //compute the position in [0,1]^3
    if(f == Face::U){
        v = QVector3D(x*delta, (s-1-y)*delta, 1);
    }
    else if(f == Face::F){
        v = QVector3D(x*delta, 0, (s-1-y)*delta);
    }
    else if(f == Face::R){
        v = QVector3D(1, x*delta, (s-1-y)*delta);
    }

    //map to [-1,1]^3
    return 2 * v - QVector3D(1, 1, 1);
}

QVector3D CubeGraphicsObject::faceCenter(Face f){
    if(f == Face::U){
        return QVector3D(0, 0, 1);
    }
    else if(f == Face::F){
        return QVector3D(0, -1, 0);
    }
    else if(f == Face::R){
        return QVector3D(1, 0, 0);
    }

    return QVector3D();
}

void CubeGraphicsObject::onMoveDone(){
    update();
}

void CubeGraphicsObject::onRotationDone(){
    update();
}

void CubeGraphicsObject::onCubeReset(){
    update();
}

void CubeGraphicsObject::onCubeScrambled(){
    update();
}

void CubeGraphicsObject::onCubeStateChanged(){
    update();
}

void CubeGraphicsObject::onCubeSizeChanged(){
    drawGuideLines();
    update();
}

void CubeGraphicsObject::onLineColourSettingChanged(){
    update();
}

void CubeGraphicsObject::onLineWidthSettingChanged(){
    update();
}

void CubeGraphicsObject::onColoursSettingChanged(){
    update();
}

void CubeGraphicsObject::onGuideLinesSettingChanged(){
    drawGuideLines();
    update();
}

void CubeGraphicsObject::onSupercubeSettingChanged(){
    update();
}

void CubeGraphicsObject::onSupercubeStickersSettingChanged(){
    update();
}

void CubeGraphicsObject::onPochmannBarThicknessSettingChanged(){
    update();
}

void CubeGraphicsObject::onPochmannCageSettingChanged(){
    update();
}
