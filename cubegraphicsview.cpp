#include "cubegraphicsview.h"

CubeGraphicsView::CubeGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    //create a graphics scene
    scene = new QGraphicsScene(this);
    setScene(scene);

    //flip the graphicsview vertically so that upwards = positive instead of negative
    scale(1, -1);

    //initialize variables
    zoomFactor = 1;
}

void CubeGraphicsView::initialize(Cube *cube, Settings *settings){
    this->cube = cube;

    //create the cube graphics object and add it to the scene
    cubeGraphicsObject = new CubeGraphicsObject(cube, settings);
    scene->addItem(cubeGraphicsObject);

    this->settings = settings;

    //read whatever settings we need from the settings object
    setBackgroundBrush(settings->getBackgroundColour());
    setRenderHint(QPainter::Antialiasing, settings->getAntialiasing());

    //connect to signals that are emitted from settings being changed
    connect(settings, SIGNAL(backgroundColourChanged()), this, SLOT(onBackgroundColourSettingChanged()));
    connect(settings, SIGNAL(antialiasingChanged()), this, SLOT(onAntialiasingSettingChanged()));

    //propogate the moveDrag signal
    connect(cubeGraphicsObject, SIGNAL(moveDrag(Axis,int,bool,Qt::MouseButton)), this, SIGNAL(moveDrag(Axis,int,bool,Qt::MouseButton)));

    //reset the scene rect when the projection is changed
    connect(cubeGraphicsObject, SIGNAL(projectionChanged()), this, SLOT(onProjectionChanged()));

    connect(cube, SIGNAL(moveDone(Move)), viewport(), SLOT(repaint()));
}

void CubeGraphicsView::zoom(qreal factor){
    zoomFactor *= factor;
    scale(factor, factor);
}

void CubeGraphicsView::setCubeProjection(QString matrix){
    QStringList list = matrix.split(",");
    if(list.length() != 6) return;

    float m[6];
    for(int i=0; i<6; i++){
        bool ok;
        m[i] = list[i].toFloat(&ok);
        if(!ok) return;
    }

    setCubeProjection(m);
}

void CubeGraphicsView::setCubeProjection(float *matrix){
    cubeGraphicsObject->setProjection(matrix);
}

void CubeGraphicsView::resetCubeProjection(){
    //isometric projection
    float m[6] = {1/sqrt(2), 1/sqrt(2), 0, -1/sqrt(6), 1/sqrt(6), sqrt(2./3)};

    cubeGraphicsObject->setProjection(m);
}

CubeGraphicsObject *CubeGraphicsView::getCubeGraphicsObject(){
    return cubeGraphicsObject;
}

void CubeGraphicsView::keyPressEvent(QKeyEvent *event){
    event->accept();

    Qt::KeyboardModifiers modifiers = event->modifiers();
    bool ctrl = modifiers & Qt::ControlModifier;

    if(event->key() == Qt::Key_PageUp){
        if(ctrl) zoom(1.01);
        else zoom(1.05);
    }
    else if(event->key() == Qt::Key_PageDown){
        if(ctrl) zoom(1/1.01);
        else zoom(1/1.05);
    }
    else if(event->key() == Qt::Key_Home){
        //reset zoom
        zoom(1/zoomFactor);
    }
    else if(event->key() == Qt::Key_I){
        QElapsedTimer t;
        t.start();
        QList<QGraphicsItem*> items = this->items(viewport()->rect());
        qDebug() << "took" << t.elapsed() << "ms";
    }
    else{
        event->ignore();
    }

    if(!event->isAccepted()){
        QGraphicsView::keyPressEvent(event);
    }
}

void CubeGraphicsView::paintEvent(QPaintEvent *event){
    QElapsedTimer t;
    t.start();

    qreal size = 350;
    int cubeSize = cube->getSize();

    QPolygonF r = mapToScene(viewport()->rect())
                * QTransform::fromScale(2/size, 2/size);
    qDebug() << r;

    Projection p = cubeGraphicsObject->proj;

    QPolygonF polyU = p.unprojectU(r)
                       .boundingRect()
                       .intersected(QRectF(-1,-1,2,2))
                       * QTransform::fromTranslate(1, 1)
                       * QTransform::fromScale(0.5, 0.5)
                       * QTransform::fromScale(cubeSize, cubeSize);
    QPolygonF polyF = p.unprojectF(r)
                       .boundingRect()
                       .intersected(QRectF(-1,-1,2,2))
                       * QTransform::fromTranslate(1, 1)
                       * QTransform::fromScale(0.5, 0.5)
                       * QTransform::fromScale(cubeSize, cubeSize);
    QPolygonF polyR = p.unprojectR(r)
                       .boundingRect()
                       .intersected(QRectF(-1,-1,2,2))
                       * QTransform::fromTranslate(1, 1)
                       * QTransform::fromScale(0.5, 0.5)
                       * QTransform::fromScale(cubeSize, cubeSize);

    QRectF rectU = polyU.boundingRect();
    QRectF rectF = polyF.boundingRect();
    QRectF rectR = polyR.boundingRect();

    qDebug() << rectU << rectF << rectR;

    int startUx = floor(rectU.left());
    int endUx = qMin((int)floor(rectU.right()), cubeSize-1);
    int startUy = floor(rectU.top());
    int endUy = qMin((int)floor(rectU.bottom()), cubeSize-1);
    int startFx = floor(rectF.left());
    int endFx = qMin((int)floor(rectF.right()), cubeSize-1);
    int startFy = floor(rectF.top());
    int endFy = qMin((int)floor(rectF.bottom()), cubeSize-1);
    int startRx = floor(rectR.left());
    int endRx = qMin((int)floor(rectR.right()), cubeSize-1);
    int startRy = floor(rectR.top());
    int endRy = qMin((int)floor(rectR.bottom()), cubeSize-1);

    qDebug() << "U:" << startUx << "to" << endUx << "and" << startUy << "to" << endUy;

    int numU = (endUx+1-startUx)*(endUy+1-startUy);
    int numF = (endFx+1-startFx)*(endFy+1-startFy);
    int numR = (endRx+1-startRx)*(endRy+1-startRy);

    qDebug() << "number of stickers to paint =" << numU+numF+numR;

    QPainter painter(viewport());
    painter.setRenderHints(painter.renderHints(), false);
    painter.setRenderHints(renderHints(), true);
    painter.setWorldTransform(viewportTransform());

    drawBackground(&painter, mapToScene(viewport()->rect()).boundingRect());

    QStyleOptionGraphicsItem o;

    for(int y=startUy; y<=endUy; y++){
        for(int x=startUx; x<=endUx; x++){
            Sticker *s = cubeGraphicsObject->stickers[Face::U][cubeSize-1-y][x];
            painter.setTransform(viewportTransform());
            painter.translate(s->pos());
            painter.setTransform(s->transform(), true);
            s->paint(&painter, &o, 0);
        }
    }

    for(int y=startFy; y<=endFy; y++){
        for(int x=startFx; x<=endFx; x++){
            Sticker *s = cubeGraphicsObject->stickers[Face::F][cubeSize-1-y][x];
            painter.setTransform(viewportTransform());
            painter.translate(s->pos());
            painter.setTransform(s->transform(), true);
            s->paint(&painter, &o, 0);
        }
    }

    for(int y=startRy; y<=endRy; y++){
        for(int x=startRx; x<=endRx; x++){
            Sticker *s = cubeGraphicsObject->stickers[Face::R][cubeSize-1-y][x];
            painter.setTransform(viewportTransform());
            painter.translate(s->pos());
            painter.setTransform(s->transform(), true);
            s->paint(&painter, &o, 0);
        }
    }

    //QGraphicsView::paintEvent(event);

    qDebug() << "Repainted the scene in" << t.elapsed() << "ms";
}

void CubeGraphicsView::onProjectionChanged(){
    //set the scene rect to the smallest rect that contains everything
    scene->setSceneRect(scene->itemsBoundingRect());
}

void CubeGraphicsView::onAntialiasingSettingChanged(){
    setRenderHint(QPainter::Antialiasing, settings->getAntialiasing());
}

void CubeGraphicsView::onBackgroundColourSettingChanged(){
    setBackgroundBrush(QBrush(settings->getBackgroundColour()));
}
