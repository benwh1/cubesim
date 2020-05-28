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
    interactionEnabled = true;

    settings = new Settings(this);
    cube = new Cube(settings, this);
    statistics = new Statistics(cube, this);
    reconstruction = new Reconstruction(cube, statistics, this);

    ui->graphicsView->initialize(cube, settings);
    ui->statisticsWidget->initialize(statistics, settings);

    replayRecorder = new ReplayRecorder(this, reconstruction, cube, statistics);

    //detect drags so we can pass the move to the cube
    connect(ui->graphicsView, SIGNAL(moveDrag(Axis,int,bool,Qt::MouseButton)), this, SLOT(onMoveDrag(Axis,int,bool,Qt::MouseButton)));

    //detect cube moves
    connect(cube, SIGNAL(moveDone(Move)), this, SLOT(onMoveDone(Move)));

    //detect when the cube is solved
    connect(cube, SIGNAL(cubeSolved()), this, SLOT(onCubeSolved()));

    //let the statistics widget know when a solve is finished so that it can
    //make the timer label green
    connect(this, SIGNAL(solveFinished()), ui->statisticsWidget, SLOT(onSolveFinished()));

    //load the settings file if it exists
    settings->load("settings.dat");

    //connect to controls signals
    connect(settings->getControls(), SIGNAL(scrambleShortcutActivated()), this, SLOT(onScrambleShortcutActivated()));
    connect(settings->getControls(), SIGNAL(resetShortcutActivated()), this, SLOT(onResetShortcutActivated()));
    connect(settings->getControls(), SIGNAL(increaseSizeShortcutActivated()), this, SLOT(onIncreaseSizeShortcutActivated()));
    connect(settings->getControls(), SIGNAL(decreaseSizeShortcutActivated()), this, SLOT(onDecreaseSizeShortcutActivated()));
    connect(settings->getControls(), SIGNAL(changeSizeShortcutActivated()), this, SLOT(onChangeSizeShortcutActivated()));

    connect(settings->getControls(), SIGNAL(loadProjectionShortcutActivated(int)), this, SLOT(onLoadProjectionShortcutActivated(int)));
    connect(settings->getControls(), SIGNAL(resetProjectionShortcutActivated()), this, SLOT(onResetProjectionShortcutActivated()));
    connect(settings->getControls(), SIGNAL(changeProjectionShortcutActivated()), this, SLOT(onChangeProjectionShortcutActivated()));

    connect(settings->getControls(), SIGNAL(resetZoomShortcutActivated()), this, SLOT(onResetZoomShortcutActivated()));
    connect(settings->getControls(), SIGNAL(zoomInShortcutActivated()), this, SLOT(onZoomInShortcutActivated()));
    connect(settings->getControls(), SIGNAL(zoomInSmallShortcutActivated()), this, SLOT(onZoomInSmallShortcutActivated()));
    connect(settings->getControls(), SIGNAL(zoomInLargeShortcutActivated()), this, SLOT(onZoomInLargeShortcutActivated()));
    connect(settings->getControls(), SIGNAL(zoomOutShortcutActivated()), this, SLOT(onZoomOutShortcutActivated()));
    connect(settings->getControls(), SIGNAL(zoomOutSmallShortcutActivated()), this, SLOT(onZoomOutSmallShortcutActivated()));
    connect(settings->getControls(), SIGNAL(zoomOutLargeShortcutActivated()), this, SLOT(onZoomOutLargeShortcutActivated()));

    connect(settings->getControls(), SIGNAL(saveShortcutActivated()), this, SLOT(onSaveShortcutActivated()));
    connect(settings->getControls(), SIGNAL(loadShortcutActivated()), this, SLOT(onLoadShortcutActivated()));

    connect(settings->getControls(), SIGNAL(toggleStatsShortcutActivated()), this, SLOT(onToggleStatsShortcutActivated()));

    connect(settings->getControls(), SIGNAL(toggleMultisliceShortcutActivated()), this, SLOT(onToggleMultisliceShortcutActivated()));

    connect(settings->getControls(), SIGNAL(screenshotShortcutActivated()), this, SLOT(onScreenshotShortcutActivated()));
}

CubeWidget::~CubeWidget()
{
    delete ui;
}

Cube *CubeWidget::getCube(){
    return cube;
}

Reconstruction *CubeWidget::getReconstruction(){
    return reconstruction;
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

Statistics *CubeWidget::getStatistics(){
    return statistics;
}

void CubeWidget::reset(){
    cube->reset();
    statistics->reset();
    reconstruction->reset();

    state = State::Neutral;
}

void CubeWidget::keyPressEvent(QKeyEvent *event){
    if(!interactionEnabled){
        return;
    }

    if(event->isAutoRepeat()){
        event->ignore();
        return;
    }

    event->accept();

    Qt::KeyboardModifiers modifiers = event->modifiers();
    bool ctrl = modifiers & Qt::ControlModifier;
    bool shift = modifiers & Qt::ShiftModifier;

    if(event->key() == Qt::Key_T){
        swapCtrlShift = !swapCtrlShift;
    }
    else if(event->key() == Qt::Key_Y){
        overlapStats = !overlapStats;

        //send a resize event to the widget
        QResizeEvent *r = new QResizeEvent(size(), size());
        QApplication::sendEvent(this, r);
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
    data["graphicsView"] = ui->graphicsView->toJSON();
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

    cube->fromJSON(data["cube"].toObject());
    ui->graphicsView->fromJSON(data["graphicsView"].toObject());
    swapCtrlShift = data["swapCtrlShift"].toBool();
    settings->fromJSON(data["settings"].toObject());
    reconstruction->fromJSON(data["reconstruction"].toObject());
    state = (State)data["state"].toInt();

    //load the statistics last, so the timer isn't running while the rest
    //of the save file is being loaded
    statistics->fromJSON(data["statistics"].toObject());
    ui->statisticsWidget->fromJSON(data["statisticsWidget"].toObject());
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

void CubeWidget::save(QString fileName){
    if(state != State::Solving && state != State::Finished) return;

    QJsonDocument document(toJSON());

    //make the directory if it doesn't already exist
    QFile f(fileName);
    QFileInfo fileInfo(f);
    QDir::current().mkpath(fileInfo.path());

    //write the file
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
    if(!interactionEnabled){
        return;
    }

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

    //do the move
    cube->move(move);
}

void CubeWidget::onMoveDone(Move move){
    //if we are in inspection, start the timer and reconstruction
    if(state == State::Inspecting){
        statistics->reset();
        statistics->startTimer();

        state = State::Solving;
    }

    //if we are solving, add the move to statistics
    if(state == State::Solving){
        statistics->doMove();
    }
}

void CubeWidget::onCubeSolved(){
    //if we were solving, stop the timer and reconstruction
    if(state == State::Solving){
        statistics->stopTimer();
        reconstruction->finish();

        state = State::Finished;

        //need to call this before saving, because some things that get saved
        //in the save file might need to be updated after the solve finishes
        //(currently, this is just the style sheet of the statistics widget)
        emit solveFinished();

        //save the solve
        //directory to save in
        QString s = QString::number(cube->getSize());
        QDir d("solves/" + s + "x" + s + "x" + s);

        //count how many solves we already have
        d.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
        int n = d.count();

        //save the solve
        save(d.filePath(QString::number(n+1) + ".dat"));
    }
}

void CubeWidget::onScrambleShortcutActivated(){
    if(state == State::Neutral || state == State::Finished){
        statistics->reset();
        reconstruction->reset();
        reconstruction->start();
        cube->scramble();
        state = State::Inspecting;
    }
}

void CubeWidget::onResetShortcutActivated(){
    reset();
}

void CubeWidget::onIncreaseSizeShortcutActivated(){
    if(state == State::Neutral){
        cube->setSize(cube->getSize()+1);
    }
}

void CubeWidget::onDecreaseSizeShortcutActivated(){
    if(state == State::Neutral){
        cube->setSize(cube->getSize()-1);
    }
}

void CubeWidget::onChangeSizeShortcutActivated(){
    if(state == State::Neutral){
        bool ok;
        QString str = QInputDialog::getText(this, "Cube size", "New cube size:", QLineEdit::Normal, QString(), &ok);
        if(!ok) return;

        int newSize = str.toInt(&ok);
        if(!ok) return;

        cube->setSize(newSize);
    }
}

void CubeWidget::onLoadProjectionShortcutActivated(int i){
    QFile f("projections.txt");
    f.open(QFile::ReadOnly);
    QList<QByteArray> arr = f.readAll().split('\n');
    f.close();

    assert(1 <= i && i <= 10);

    if(i-1 < arr.length()){
        ui->graphicsView->setCubeProjection(arr[i-1]);
    }
}

void CubeWidget::onResetProjectionShortcutActivated(){
    ui->graphicsView->resetCubeProjection();
}

void CubeWidget::onChangeProjectionShortcutActivated(){
    bool ok;
    QString str = QInputDialog::getText(this, "Projection", "Projection:", QLineEdit::Normal, QString(), &ok);
    if(!ok) return;

    ui->graphicsView->setCubeProjection(str);
}

void CubeWidget::onResetZoomShortcutActivated(){
    ui->graphicsView->resetZoom();
}

void CubeWidget::onZoomInShortcutActivated(){
    ui->graphicsView->zoom(1.05);
}

void CubeWidget::onZoomInSmallShortcutActivated(){
    ui->graphicsView->zoom(1.01);
}

void CubeWidget::onZoomInLargeShortcutActivated(){
    ui->graphicsView->zoom(1.25);
}

void CubeWidget::onZoomOutShortcutActivated(){
    ui->graphicsView->zoom(1/1.05);
}

void CubeWidget::onZoomOutSmallShortcutActivated(){
    ui->graphicsView->zoom(1/1.01);
}

void CubeWidget::onZoomOutLargeShortcutActivated(){
    ui->graphicsView->zoom(1/1.25);
}

void CubeWidget::onSaveShortcutActivated(){
    save();
}

void CubeWidget::onLoadShortcutActivated(){
    load();
}

void CubeWidget::onToggleStatsShortcutActivated(){
    ui->statisticsWidget->setVisible(!ui->statisticsWidget->isVisible());
}

void CubeWidget::onToggleMultisliceShortcutActivated(){
    settings->setMultislice(!settings->getMultislice());
}

void CubeWidget::onScreenshotShortcutActivated(){
    //render the image
    QImage image(size(), QImage::Format_ARGB32);
    QPainter painter(&image);
    render(&painter);

    //make images directory if needed
    QDir::current().mkdir("images");

    //save the image
    QString name = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
    image.save("images/" + name + ".png", "PNG");
}
