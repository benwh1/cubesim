#include "cubewidget.h"
#include "ui_cubewidget.h"

#include "replayrecorder.h"

CubeWidget::CubeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CubeWidget),
    canDoMove(true)
{
    ui->setupUi(this);

    setState(State::Neutral);

    settings = new Settings(this);
    cube = new Cube(settings, this);
    statistics = new Statistics(cube, this);
    reconstruction = new Reconstruction(cube, statistics, this);

    ui->graphicsView->initialize(cube, settings);
    ui->statisticsWidget->initialize(statistics, settings);

    replayRecorder = new ReplayRecorder(this, reconstruction, cube, statistics);

    canDoMove.setDuration(settings->getControls()->getMinMoveDuration());

    //resize the window when the replay recorder setting is changed
    connect(replayRecorder->getSettings(), SIGNAL(settingChanged()), this, SLOT(onReplayRecorderSettingChanged()));

    //detect drags so we can pass the move to the cube
    connect(ui->graphicsView, SIGNAL(moveDrag(Axis,int,bool,Qt::MouseButton)), this, SLOT(onMoveDrag(Axis,int,bool,Qt::MouseButton)));

    //detect cube moves
    connect(cube, SIGNAL(moveDone(Move)), this, SLOT(onMoveDone()));

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

    connect(settings->getControls(), SIGNAL(minMoveDurationChanged()), this, SLOT(onMinMoveDurationChanged()));
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

    setState(State::Neutral);
}

void CubeWidget::resizeEvent(QResizeEvent *event){
    int size = 220;
    ui->graphicsView->setGeometry(0, 0, event->size().width(), event->size().height());

    //the statistics widget is not in a layout, which means the sizePolicy is
    //ignored. we want the widget to be the minimum height, so we set it here
    ui->statisticsWidget->setGeometry(0, 0, size, ui->statisticsWidget->sizeHint().height());
}

QJsonObject CubeWidget::toJSON(){
    QJsonObject data;

    data["version"] = Version::saveFormatVersion.toString();
    data["statistics"] = statistics->toJSON();
    data["cube"] = cube->toJSON();
    data["graphicsView"] = ui->graphicsView->toJSON();
    data["settings"] = settings->toJSON();
    data["reconstruction"] = reconstruction->toJSON();
    data["state"] = state;
    data["statisticsWidget"] = ui->statisticsWidget->toJSON();

    return data;
}

void CubeWidget::fromJSON(QJsonObject data){
    //can't load save files from newer versions
    QVersionNumber saveVersion = QVersionNumber::fromString(data["version"].toString());
    if(saveVersion > Version::saveFormatVersion){
        QMessageBox::warning(this, "Error", "Can't load save format version v" + saveVersion.toString());
        return;
    }

    //backwards compatibility
    data = SaveConverter::convert(data);

    cube->fromJSON(data["cube"].toObject());
    ui->graphicsView->fromJSON(data["graphicsView"].toObject());
    settings->fromJSON(data["settings"].toObject());
    reconstruction->fromJSON(data["reconstruction"].toObject());
    setState((State)data["state"].toInt());

    //load the statistics last, so the timer isn't running while the rest
    //of the save file is being loaded
    statistics->fromJSON(data["statistics"].toObject());
    ui->statisticsWidget->fromJSON(data["statisticsWidget"].toObject());

    //ugly hack to make statisticsWidget be blank if we load a save file in
    //the neutral or inspecting state
    //TODO: fix this. maybe statisticsWidget should save the current text?
    if(state == State::Neutral || state == State::Inspecting){
        ui->statisticsWidget->clear();
    }
}

void CubeWidget::save(){
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
}

void CubeWidget::setState(State s){
    state = s;
    emit stateChanged();
}

void CubeWidget::onMoveDrag(Axis axis, int layer, bool clockwise, Qt::MouseButton button){
    /* there are five different types of click:
     * - left click
     * - ctrl click
     * - shift click
     * - right click
     * - middle click
     * which can apply any of the three possible modifiers:
     * - quarter turn
     * - half turn
     * - rotation
     * we want to be able to combine them. e.g. if the quarter turn and
     * half turn modifiers are both active, then we should do a half turn.
     * half turn and rotation can be combined, but we want half turn and
     * rotation to each (separately) override quarter turn.
     */

    if(!canDoMove){
        return;
    }

    Controls *c = settings->getControls();

    //if the mouse button we used is set to do nothing, return immediately
    if((button == Qt::LeftButton && c->getLeftClickAction() == MoveType::None) ||
       (button == Qt::RightButton && c->getRightClickAction() == MoveType::None) ||
       (button == Qt::MiddleButton && c->getMiddleClickAction() == MoveType::None)){
        return;
    }

    //check which move modifiers are being used
    Qt::KeyboardModifiers keyboardModifiers = QGuiApplication::queryKeyboardModifiers();
    bool ctrl = keyboardModifiers & Qt::ControlModifier;
    bool shift = keyboardModifiers & Qt::ShiftModifier;

    QHash<MoveType, bool> moveModifiers;

    moveModifiers[c->getLeftClickAction()] |= (button == Qt::LeftButton);
    moveModifiers[c->getCtrlClickAction()] |= ctrl;
    moveModifiers[c->getShiftClickAction()] |= shift;
    moveModifiers[c->getRightClickAction()] |= (button == Qt::RightButton);
    moveModifiers[c->getMiddleClickAction()] |= (button == Qt::MiddleButton);

    //compute the amount (cw = 1, half = 2, ccw = 3)
    int amount;

    //if half turn modifier, always do a half turn
    if(moveModifiers[MoveType::HalfTurn]){
        amount = 2;
    }
    //otherwise, check if quarter turn modifier or rotation is used.
    //rotation overrides quarter turn, so even if quarter turn is not used,
    //we should still do a quarter rotation
    else if(moveModifiers[MoveType::QuarterTurn] ||
            moveModifiers[MoveType::Rotation]){
        //check if cw or ccw
        if(clockwise){
            amount = 1;
        }
        else{
            amount = 3;
        }
    }
    //no move modifiers used, so we don't need to do anything
    else{
        return;
    }

    //check if the move is a rotation
    bool isRotation = moveModifiers[MoveType::Rotation];

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

void CubeWidget::onMoveDone(){
    //if we are in inspection, start the timer and reconstruction
    if(state == State::Inspecting){
        statistics->reset();
        statistics->startTimer();

        setState(State::Solving);
    }

    //if we are solving, add the move to statistics
    if(state == State::Solving){
        statistics->doMove();
    }

    //don't allow another move until a certain amount of time has passed
    canDoMove = false;
}

void CubeWidget::onCubeSolved(){
    //if we were solving, stop the timer and reconstruction
    if(state == State::Solving){
        statistics->stopTimer();
        reconstruction->finish();

        setState(State::Finished);

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

        setState(State::Inspecting);
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
    //only load when we aren't doing a solve
    if(state == State::Neutral){
        load();
    }
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

void CubeWidget::onMinMoveDurationChanged(){
    int d = settings->getControls()->getMinMoveDuration();
    canDoMove.setDuration(d);
}

void CubeWidget::onReplayRecorderSettingChanged(){
    //resize the CubeWidget to the size in the replay recorder settings
    resize(replayRecorder->getSettings()->getVideoSize());
}
