#include "cubewidget.h"
#include "ui_cubewidget.h"

CubeWidget::CubeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CubeWidget)
{
    ui->setupUi(this);

    //initialize variables
    state = State::Neutral;
    swapCtrlShift = false;
    settings = new Settings(this);
    statistics = new Statistics(this);

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
}

void CubeWidget::initialize(Cube *cube){
    this->cube = cube;

    ui->graphicsView->initialize(cube, settings);
    ui->statisticsWidget->initialize(statistics, settings);

    //detect drags so we can pass the move to the cube
    connect(ui->graphicsView, SIGNAL(moveDrag(Axis,int,bool,Qt::MouseButton)), this, SLOT(onMoveDrag(Axis,int,bool,Qt::MouseButton)));

    //detect cube moves
    connect(cube, SIGNAL(moveDone(Axis,int,int,int)), this, SLOT(onMoveDone(Axis,int,int,int)));

    //detect when the cube is solved
    connect(cube, SIGNAL(cubeSolved()), this, SLOT(onCubeSolved()));
    connect(cube, SIGNAL(cubeSolved()), ui->statisticsWidget, SLOT(onCubeSolved()));
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
            reconstruction.reset();
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
    else{
        event->ignore();
    }

    if(!event->isAccepted()){
        QWidget::keyPressEvent(event);
    }
}

void CubeWidget::resizeEvent(QResizeEvent *event){
    ui->graphicsView->setGeometry(0, 0, event->size().width(), event->size().height());

    //the statistics widget is not in a layout, which means the sizePolicy is
    //ignored. we want the widget to be the minimum height, so we set it here
    ui->statisticsWidget->setGeometry(0, 0, 200, ui->statisticsWidget->sizeHint().height());

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
}

void CubeWidget::save(){
    if(state != State::Solving) return;

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

    //read the file
    QFile f(name);
    f.open(QFile::ReadOnly);
    QByteArray data = f.readAll();
    f.close();

    //load the save data
    QJsonDocument document = QJsonDocument::fromBinaryData(data);
    fromJSON(document.object());

    //set the state to solving
    state = State::Solving;
}

void CubeWidget::onMoveDrag(Axis axis, int layer, bool clockwise, Qt::MouseButton button){
    Qt::KeyboardModifiers modifiers = QGuiApplication::queryKeyboardModifiers();

    bool ctrl = modifiers & Qt::ControlModifier;
    bool shift = modifiers & Qt::ShiftModifier;

    bool halfTurn, rotation;
    if(swapCtrlShift){
        halfTurn = ctrl;
        rotation = shift;
    }
    else{
        halfTurn = shift;
        rotation = ctrl;
    }

    int amount;

    //use right mouse button for half turns
    if(halfTurn || button == Qt::RightButton) amount = 2;
    else if(clockwise) amount = 1;
    else amount = 3;

    //don't allow moves if the solve is finished
    if(state != State::Finished){
        if(rotation) cube->rotate(axis, amount);
        else if(settings->getMultislice()) cube->multisliceMove(axis, layer, amount);
        else cube->move(axis, layer, amount);
    }
}

void CubeWidget::onMoveDone(Axis axis, int layerStart, int layerEnd, int amount){
    //if we are in inspection, start the timer
    if(state == State::Inspecting){
        statistics->reset();
        statistics->startTimer();
        state = State::Solving;
    }

    //if we are solving, add the move to statistics and reconstruction
    if(state == State::Solving){
        qint64 time = statistics->getTime();
        reconstruction.addMove(axis, layerStart, layerEnd, amount, time);
        statistics->doMove();
    }
}

void CubeWidget::onCubeSolved(){
    //if we were solving, stop the timer
    if(state == State::Solving){
        statistics->stopTimer();
        state = State::Finished;
    }
}
