#include "cubegraphicsview.h"

CubeGraphicsView::CubeGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);

    //setRenderHint(QPainter::Antialiasing);
    setBackgroundBrush(QBrush(QColor(255, 228, 196)));

    //flip the graphicsview vertically so that upwards = positive instead of negative
    scale(1, -1);
}

void CubeGraphicsView::initialize(Cube *cube){
    this->cube = cube;
    cubeGraphicsObject = new CubeGraphicsObject(cube);
    scene->addItem(cubeGraphicsObject);
}

void CubeGraphicsView::keyReleaseEvent(QKeyEvent *event){
    if(event->isAutoRepeat()){
        event->ignore();
        return;
    }

    if(event->key() == Qt::Key_Space){
        cube->scramble();
    }
    else if(event->key() == Qt::Key_Escape){
        cube->reset();
    }
    else if(event->key() == Qt::Key_PageUp){
        scale(1.25, 1.25);
    }
    else if(event->key() == Qt::Key_PageDown){
        scale(1/1.25, 1/1.25);
    }

    event->accept();
}
