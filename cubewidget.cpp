#include "cubewidget.h"
#include "ui_cubewidget.h"

#include "replayrecorder.h"

CubeWidget::CubeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CubeWidget)
{
    ui->setupUi(this);

    //initialize variables
    state = State::Neutral;
    swapCtrlShift = false;
    overlapStats = true;

    reconstruction = new Reconstruction();
    settings = new Settings(this);
    statistics = new Statistics(this);

    cube = new Cube(settings, this);

    ui->graphicsView->initialize(cube, settings);
    ui->statisticsWidget->initialize(statistics, settings);

    replayRecorder = new ReplayRecorder(this, reconstruction, cube, statistics);

    //detect drags so we can pass the move to the cube
    connect(ui->graphicsView, SIGNAL(moveDrag(Axis,int,bool,Qt::MouseButton)), this, SLOT(onMoveDrag(Axis,int,bool,Qt::MouseButton)));

    //detect cube moves
    connect(cube, SIGNAL(moveDone(Move)), this, SLOT(onMoveDone(Move)));
    connect(cube, SIGNAL(rotationDone(Move)), this, SLOT(onRotationDone(Move)));

    //detect when the cube is solved
    connect(cube, SIGNAL(cubeSolved()), this, SLOT(onCubeSolved()));
    connect(cube, SIGNAL(cubeSolved()), ui->statisticsWidget, SLOT(onCubeSolved()));

    //load the settings file if it exists
    QFile f("settings.dat");
    if(f.exists()){
        f.open(QFile::ReadOnly);
        QJsonDocument document = QJsonDocument::fromBinaryData(f.readAll());
        f.close();

        QJsonObject data = document.object();
        settings->fromJSON(data);
    }
}

CubeWidget::~CubeWidget()
{
    delete ui;
    delete reconstruction;
}

Cube *CubeWidget::getCube(){
    return cube;
}

ReplayRecorder *CubeWidget::getReplayRecorder(){
    return replayRecorder;
}

Settings *CubeWidget::getSettings(){
    return settings;
}

CubeWidget::State CubeWidget::getState(){
    return state;
}

void CubeWidget::keyPressEvent(QKeyEvent *event){
    if(event->isAutoRepeat()){
        event->ignore();
        return;
    }

    event->accept();

    Qt::KeyboardModifiers modifiers = event->modifiers();
    bool ctrl = modifiers & Qt::ControlModifier;
    bool shift = modifiers & Qt::ShiftModifier;

    if(event->key() == Qt::Key_Space){
        if(state == State::Neutral || state == State::Finished){
            reconstruction->reset();
            cube->scramble();
            state = State::Inspecting;
        }
    }
    else if(event->key() == Qt::Key_Escape){
        cube->reset();
        statistics->reset();
        state = State::Neutral;
    }
    else if(Qt::Key_0 <= event->key() && event->key() <= Qt::Key_9){
        QFile f("projections.txt");
        f.open(QFile::ReadOnly);
        QList<QByteArray> arr = f.readAll().split('\n');
        f.close();

        int n = event->key() - Qt::Key_0;
        if(arr.length() <= n) return;
        ui->graphicsView->setCubeProjection(arr[n]);
    }
    else if(event->key() == Qt::Key_D){
        ui->graphicsView->resetCubeProjection();
    }
    else if(event->key() == Qt::Key_O){
        if(ctrl){
            load();
        }
    }
    else if(event->key() == Qt::Key_P){
        bool ok;
        QString str = QInputDialog::getText(this, "Projection", "Projection:", QLineEdit::Normal, QString(), &ok);
        if(!ok) return;

        ui->graphicsView->setCubeProjection(str);
    }
    else if(event->key() == Qt::Key_T){
        swapCtrlShift = !swapCtrlShift;
    }
    else if(event->key() == Qt::Key_S){
        if(ctrl){
            save();
        }
    }
    else if(event->key() == Qt::Key_Y){
        overlapStats = !overlapStats;

        //send a resize event to the widget
        QResizeEvent *r = new QResizeEvent(size(), size());
        QApplication::sendEvent(this, r);
    }
    else if(event->key() == Qt::Key_V){
        //toggle visibility of stats box
        ui->statisticsWidget->setVisible(!ui->statisticsWidget->isVisible());
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
    else if(event->key() == Qt::Key_CapsLock){
        settings->setMultislice(!settings->getMultislice());
    }
    else if(event->key() == Qt::Key_L){
        QMessageBox::information(this, "Reconstruction", reconstruction->toString());
    }
    else{
        event->ignore();
    }

    if(!event->isAccepted()){
        QWidget::keyPressEvent(event);
    }
}

void CubeWidget::resizeEvent(QResizeEvent *event){
    int size = 220;
    int n = overlapStats ? 0 : size;
    ui->graphicsView->setGeometry(n, 0, event->size().width()-n, event->size().height());

    //the statistics widget is not in a layout, which means the sizePolicy is
    //ignored. we want the widget to be the minimum height, so we set it here
    ui->statisticsWidget->setGeometry(0, 0, size, ui->statisticsWidget->sizeHint().height());

    event->accept();
}

QJsonObject CubeWidget::toJSON(){
    QJsonObject data;

    data["version"] = Global::saveFormatVersion();
    data["statistics"] = statistics->toJSON();
    data["cube"] = cube->toJSON();
    data["cubeGraphicsObject"] = ui->graphicsView->getCubeGraphicsObject()->toJSON();
    data["swapCtrlShift"] = swapCtrlShift;
    data["settings"] = settings->toJSON();
    data["reconstruction"] = reconstruction->toJSON();
    data["state"] = state;
    data["statisticsWidget"] = ui->statisticsWidget->toJSON();

    return data;
}

void CubeWidget::fromJSON(QJsonObject data){
    //make sure the save file is compatible with the latest format
    data = SaveConverter::convert(data);

    //TODO: make sure that the cube graphics object is only redrawn once
    //after everything has been loaded

    statistics->fromJSON(data["statistics"].toObject());
    cube->fromJSON(data["cube"].toObject());
    ui->graphicsView->getCubeGraphicsObject()->fromJSON(data["cubeGraphicsObject"].toObject());
    swapCtrlShift = data["swapCtrlShift"].toBool();
    settings->fromJSON(data["settings"].toObject());
    reconstruction->fromJSON(data["reconstruction"].toObject());
    state = (State)data["state"].toInt();
    ui->statisticsWidget->fromJSON(data["statisticsWidget"].toObject());

    if(state == State::Solving){
        statistics->startTimer();
    }
}

void CubeWidget::save(){
    if(state != State::Solving && state != State::Finished) return;

    QJsonDocument document(toJSON());

    //make save directory if it doesn't already exist
    QDir::current().mkdir("save");

    //get the save file name
    QString path = QDir("save").absolutePath();
    QString name = QFileDialog::getSaveFileName(this, "Save", path, "*.dat");
    if(name == "") return;

    //write the file
    QFile f(name);
    f.open(QFile::WriteOnly);
    f.write(document.toBinaryData());
    f.close();
}

void CubeWidget::load(){
    //only load when we aren't doing a solve
    if(state != State::Neutral) return;

    //get the save file name
    QDir dir("save");
    QString path = dir.absolutePath();
    QString name = QFileDialog::getOpenFileName(this, "Open", path, "*.dat");
    if(name == "") return;

    //load the file
    load(name);
}

void CubeWidget::load(QString fileName){
    //read the file
    QFile f(fileName);
    f.open(QFile::ReadOnly);
    QByteArray data = f.readAll();
    f.close();

    //load the save data
    QJsonDocument document = QJsonDocument::fromBinaryData(data);
    fromJSON(document.object());

    if(cube->isSolved()){
        state = State::Finished;
    }
    else{
        //set the state to solving
        state = State::Solving;
    }
}

void CubeWidget::onMoveDrag(Axis axis, int layer, bool clockwise, Qt::MouseButton button){
    QElapsedTimer t;
    t.start();

    Qt::KeyboardModifiers modifiers = QGuiApplication::queryKeyboardModifiers();

    bool ctrl = modifiers & Qt::ControlModifier;
    bool shift = modifiers & Qt::ShiftModifier;

    //check whether the move is a half turn or a rotation
    bool isHalfTurn, isRotation;
    if(swapCtrlShift){
        isHalfTurn = ctrl;
        isRotation = shift;
    }
    else{
        isHalfTurn = shift;
        isRotation = ctrl;
    }

    //use middle mouse button for rotations
    if(button == Qt::MiddleButton) isRotation = true;

    //compute the amount (cw = 1, half = 2, ccw = 3)
    int amount;

    //use right mouse button for half turns
    if(isHalfTurn || button == Qt::RightButton) amount = 2;
    else if(clockwise) amount = 1;
    else amount = 3;

    //make the Move object
    Move move;

    if(isRotation){
        move = Move(axis, 0, -1, amount);
    }
    else if(settings->getMultislice()){
        //compute which half of the cube the move was in, and compute
        //the start and end layers
        int size = cube->getSize();
        int layerStart, layerEnd;

        //U/F/R half of the cube
        if(layer < size/2){
            layerStart = 0;
            layerEnd = layer;
        }
        //middle slice on an odd cube
        else if(size%2 == 1 && layer == (size-1)/2){
            layerStart = layer;
            layerEnd = layer;
        }
        //B/L/D half of the cube
        else{
            layerStart = layer;
            layerEnd = size-1;
        }

        move = Move(axis, layerStart, layerEnd, amount);
    }
    //single slice move
    else{
        move = Move(axis, layer, layer, amount);
    }

    //if the solve is finished, only allow rotations
    if(state == State::Finished && !move.isRotation()){
        return;
    }

    qDebug() << "Processed move drag in" << t.elapsed() << "ms";

    //do the move
    cube->move(move);
}

void CubeWidget::onMoveDone(Move move){
    //if we are in inspection, start the timer
    if(state == State::Inspecting){
        statistics->reset();
        statistics->startTimer();
        state = State::Solving;
    }

    //if we are solving, add the move to statistics and reconstruction
    if(state == State::Solving){
        qint64 time = statistics->getTime();
        reconstruction->addMove(move, time);
        statistics->doMove();
    }
}

void CubeWidget::onRotationDone(Move move){
    //if we are solving, add the rotation to the reconstruction
    if(state == State::Solving){
        qint64 time = statistics->getTime();
        reconstruction->addRotation(move, time);
    }
    //if we rotate during inspection, add the rotation with a time of 0
    else if(state == State::Inspecting){
        reconstruction->addRotation(move, 0);
    }
}

void CubeWidget::onCubeSolved(){
    //if we were solving, stop the timer
    if(state == State::Solving){
        statistics->stopTimer();
        state = State::Finished;
    }
}
