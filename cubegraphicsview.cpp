#include "cubegraphicsview.h"

CubeGraphicsView::CubeGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    //create a graphics scene
    scene = new QGraphicsScene(this);
    setScene(scene);

    //setRenderHint(QPainter::Antialiasing);
    setBackgroundBrush(QBrush(QColor(255, 228, 196)));

    //flip the graphicsview vertically so that upwards = positive instead of negative
    scale(1, -1);

    //initialize variables
    zoomFactor = 1;
}

void CubeGraphicsView::initialize(Cube *cube){
    this->cube = cube;
    cubeGraphicsObject = new CubeGraphicsObject(cube);
    scene->addItem(cubeGraphicsObject);

    //propogate the moveDrag signal
    connect(cubeGraphicsObject, SIGNAL(moveDrag(Cube::Axis,int,bool)), this, SIGNAL(moveDrag(Cube::Axis,int,bool)));

    //reset the scene rect when the projection is changed
    connect(cubeGraphicsObject, SIGNAL(projectionChanged()), this, SLOT(onProjectionChanged()));
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

void CubeGraphicsView::onProjectionChanged(){
    //set the scene rect to the smallest rect that contains everything
    scene->setSceneRect(scene->itemsBoundingRect());
}
