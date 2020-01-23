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

    event->accept();

    if(event->key() == Qt::Key_Space){
        cube->scramble();
    }
    else if(event->key() == Qt::Key_Escape){
        cube->reset();
    }
    else if(event->key() == Qt::Key_P){
        bool ok;
        QString str = QInputDialog::getText(this, "Projection", "Projection:", QLineEdit::Normal, QString(), &ok);
        if(!ok) return;

        QStringList list = str.split(",");
        if(list.length() != 6) return;

        float f[6];
        for(int i=0; i<6; i++){
            f[i] = list[i].toFloat(&ok);
            if(!ok) return;
        }

        cubeGraphicsObject->setProjection(f);
    }
    else if(event->key() == Qt::Key_Equal){
        cube->setSize(cube->getSize()+1);
    }
    else if(event->key() == Qt::Key_Minus){
        cube->setSize(cube->getSize()-1);
    }
    else if(event->key() == Qt::Key_PageUp){
        scale(1.25, 1.25);
    }
    else if(event->key() == Qt::Key_PageDown){
        scale(1/1.25, 1/1.25);
    }
}
