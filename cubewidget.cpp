#include "cubewidget.h"
#include "ui_cubewidget.h"

CubeWidget::CubeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CubeWidget)
{
    ui->setupUi(this);

    //initialize variables
    multislice = false;
}

CubeWidget::~CubeWidget()
{
    delete ui;
}

void CubeWidget::initialize(Cube *cube){
    this->cube = cube;
    ui->graphicsView->initialize(cube);

    //detect drags and do the corresponding move
    connect(ui->graphicsView, SIGNAL(moveDrag(Cube::Axis,int,bool)), this, SLOT(onMoveDrag(Cube::Axis,int,bool)));
}


void CubeWidget::keyPressEvent(QKeyEvent *event){
    if(event->isAutoRepeat()){
        event->ignore();
        return;
    }

    event->accept();

    if(event->key() == Qt::Key_CapsLock){
        multislice = !multislice;
    }
}

void CubeWidget::keyReleaseEvent(QKeyEvent *event){
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
    else if(event->key() == Qt::Key_D){
        ui->graphicsView->resetCubeProjection();
    }
    else if(event->key() == Qt::Key_P){
        bool ok;
        QString str = QInputDialog::getText(this, "Projection", "Projection:", QLineEdit::Normal, QString(), &ok);
        if(!ok) return;

        ui->graphicsView->setCubeProjection(str);
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
        ui->graphicsView->zoom(1.25);
    }
    else if(event->key() == Qt::Key_PageDown){
        ui->graphicsView->zoom(1/1.25);
    }
}

void CubeWidget::onMoveDrag(Cube::Axis axis, int layer, bool clockwise){
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
