#include "cubewidget.h"
#include "ui_cubewidget.h"

CubeWidget::CubeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CubeWidget)
{
    ui->setupUi(this);

    //initialize variables
    multislice = false;
    state = State::Neutral;

    statistics = new Statistics(this);
    ui->statisticsWidget->initialize(statistics);
}

CubeWidget::~CubeWidget()
{
    delete ui;
}

void CubeWidget::initialize(Cube *cube){
    this->cube = cube;
    ui->graphicsView->initialize(cube);

    //detect drags so we can pass the move to the cube
    connect(ui->graphicsView, SIGNAL(moveDrag(Cube::Axis,int,bool)), this, SLOT(onMoveDrag(Cube::Axis,int,bool)));

    //detect cube moves
    connect(cube, SIGNAL(moveDone(Cube::Axis,int)), this, SLOT(onMoveDone(Cube::Axis,int)));

    //detect when the cube is solved
    connect(cube, SIGNAL(cubeSolved()), this, SLOT(onCubeSolved()));
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
        if(state == State::Neutral){
            cube->scramble();
            state = State::Inspecting;
        }
    }
    else if(event->key() == Qt::Key_Escape){
        cube->reset();
        statistics->reset();
        state = State::Neutral;
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
        if(state == State::Neutral){
            cube->setSize(cube->getSize()+1);
        }
    }
    else if(event->key() == Qt::Key_Plus){ //+ is shift and =
        if(shift){
            if(state == State::Neutral){
                bool ok;
                QString str = QInputDialog::getText(this, "Cube size", "New cube size:", QLineEdit::Normal, QString(), &ok);
                if(!ok) return;

                int newSize = str.toInt(&ok);
                if(!ok) return;

                cube->setSize(newSize);
            }
        }
    }
    else if(event->key() == Qt::Key_Minus){
        if(state == State::Neutral){
            cube->setSize(cube->getSize()-1);
        }
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

    //don't allow moves if the solve is finished
    if(state != State::Finished){
        if(ctrl) cube->rotate(axis, amount);
        else if(multislice) cube->multisliceMove(axis, layer, amount);
        else cube->move(axis, layer, amount);
    }
}

void CubeWidget::onMoveDone(Cube::Axis, int){
    //if we are in inspection, start the timer
    if(state == State::Inspecting){
        statistics->reset();
        statistics->startTimer();
        state = State::Solving;
    }

    statistics->doMove();
}

void CubeWidget::onCubeSolved(){
    //if we were solving, stop the timer
    if(state == State::Solving){
        statistics->stopTimer();
        state = State::Finished;
    }
}
