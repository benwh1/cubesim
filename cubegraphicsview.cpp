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

    //we need to repaint the whole viewport after the projection is changed
    //because changing the projection also changes the boundingRect of the
    //cubeGraphicsObject, so when the cubeGraphicsObject is repainted, there
    //can be visual artifacts remaining if the new boundingRect is fully
    //contained inside the old boundingRect
    viewport()->repaint();
}

void CubeGraphicsView::resetCubeProjection(){
    //isometric projection
    float m[6] = {1/sqrt(2), 1/sqrt(2), 0, -1/sqrt(6), 1/sqrt(6), sqrt(2./3)};

    cubeGraphicsObject->setProjection(m);
    viewport()->repaint();
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

    //tell the cubeGraphicsObject which part of the scene is visible
    QRectF r = mapToScene(viewport()->rect()).boundingRect();
    cubeGraphicsObject->setVisibleRect(r);

    QGraphicsView::paintEvent(event);

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
