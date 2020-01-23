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

    //initialize variables
    multislice = false;
}

void CubeGraphicsView::initialize(Cube *cube){
    this->cube = cube;
    cubeGraphicsObject = new CubeGraphicsObject(cube);
    scene->addItem(cubeGraphicsObject);

    //detect drags and do the corresponding move
    connect(cubeGraphicsObject, SIGNAL(moveDrag(Cube::Axis,int,bool)), this, SLOT(onMoveDrag(Cube::Axis,int,bool)));

    //reset the scene rect when the projection is changed
    connect(cubeGraphicsObject, SIGNAL(projectionChanged()), this, SLOT(onProjectionChanged()));
}

void CubeGraphicsView::keyPressEvent(QKeyEvent *event){
    if(event->isAutoRepeat()){
        event->ignore();
        return;
    }

    if(event->key() == Qt::Key_CapsLock){
        multislice = !multislice;
    }
}

void CubeGraphicsView::keyReleaseEvent(QKeyEvent *event){
    if(event->isAutoRepeat()){
        event->ignore();
        return;
    }

    event->accept();

    Qt::KeyboardModifiers modifiers = event->modifiers();
    bool shift = modifiers & Qt::ShiftModifier;

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
    else if(event->key() == Qt::Key_Plus){ //+ is shift and =
        if(shift){
            bool ok;
            QString str = QInputDialog::getText(this, "Cube size", "New cube size:", QLineEdit::Normal, QString(), &ok);
            if(!ok) return;

            int newSize = str.toInt(&ok);
            if(!ok) return;

            cube->setSize(newSize);
        }
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

void CubeGraphicsView::onProjectionChanged(){
    //set the scene rect to the smallest rect that contains everything
    scene->setSceneRect(scene->itemsBoundingRect());
}

void CubeGraphicsView::onMoveDrag(Cube::Axis axis, int layer, bool clockwise){
    Qt::KeyboardModifiers modifiers = QGuiApplication::queryKeyboardModifiers();

    bool ctrl = modifiers & Qt::ControlModifier;
    bool shift = modifiers & Qt::ShiftModifier;

    int amount;

    if(shift) amount = 2;
    else if(clockwise) amount = 1;
    else amount = 3;

    if(ctrl) cube->rotate(axis, amount);
    else if(multislice) cube->multisliceMove(axis, layer, amount);
    else cube->move(axis, layer, amount);
}
