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
    overlapStats = true;
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
    connect(cube, SIGNAL(rotationDone(Axis,int)), this, SLOT(onRotationDone(Axis,int)));

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
    else if(event->key() == Qt::Key_Y){
        overlapStats = !overlapStats;

        //send a resize event to the widget
        QResizeEvent *r = new QResizeEvent(size(), size());
        QApplication::sendEvent(this, r);
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
        QMessageBox::information(this, "Reconstruction", reconstruction.toString());
    }
    else if(event->key() == Qt::Key_R){
        if(ctrl){
            if(state != State::Finished){
                return;
            }

            //store these for later
            qint64 time = statistics->getTime();
            qint64 moves = statistics->getMoves();

            //reset the statistics widget to be blank
            ui->statisticsWidget->clear();

            //block all signals from the cube so that the CubeGraphicsObject
            //doesn't try to update the cube after every single move
            cube->blockSignals(true);

            //create an image and a painter
            QImage image(size(), QImage::Format_ARGB32);
            QPainter painter(&image);

            //framerate and number of frames in the video
            int fps = 30;
            qreal msPerFrame = 1000.0/fps;
            qint64 numFrames = time/msPerFrame+1;

            //which move are we currently at in the reconstruction?
            int moveNumber = 0; //including rotations
            int moveCounter = 0; //not including rotations

            //start a timer
            QElapsedTimer timer;
            timer.start();

            //scramble the cube
            cube->setState(cube->getLastScramble());

            //apply the rotations from inspection
            while(true){
                Move move = reconstruction.getMove(moveNumber);
                if(!move.isRotation()) break;
                cube->move(move);
                moveNumber++;
            }

            //render the first frame
            ui->graphicsView->getCubeGraphicsObject()->updateAll();
            render(&painter);
            image.save("images/frame_start.png");

            //render the frames
            for(int frame=0; frame<numFrames; frame++){
                //how much time has passed so far?
                qint64 timeElapsed = frame * msPerFrame;

                //do moves until we reach the current time
                QPair<Move, qint64> pair;
                while(true){
                    //check if we're at the end of the reconstruction
                    if(moveNumber >= reconstruction.length()) break;

                    //get the next move, and check if we are still within
                    //the next frame to be rendered
                    pair = reconstruction.at(moveNumber);
                    if(pair.second > timeElapsed) break;

                    //check the move after the next move, and see if it is the
                    //inverse of the next move and that both moves are within
                    //the current frame. if so, skip over both moves
                    bool skipTwoMoves;
                    if(moveNumber+1 < reconstruction.length()){
                        QPair<Move, qint64> pair2 = reconstruction.at(moveNumber+1);

                        //second move is after the current frame
                        if(pair2.second > timeElapsed){
                            skipTwoMoves = false;
                        }
                        //second move is still in the current frame
                        else{
                            //the second move is the inverse of the next move
                            if(pair.first == pair2.first.inverse()){
                                skipTwoMoves = true;
                            }
                            //the second move is not the inverse
                            else{
                                skipTwoMoves = false;
                            }
                        }
                    }
                    //the next move is the last move of the reconstruction
                    else{
                        skipTwoMoves = false;
                    }

                    //check if we need to skip two moves
                    if(skipTwoMoves){
                        moveNumber += 2;

                        //check whether the moves were rotations
                        if(!pair.first.isRotation()){
                            moveCounter += 2;
                        }

                        qDebug() << "skipping two moves";
                        continue;
                    }

                    cube->move(pair.first);

                    moveNumber++;
                    if(!pair.first.isRotation()){
                        moveCounter++;
                    }

                    pair = reconstruction.at(moveNumber);
                }

                //update the statistics
                statistics->setTime(timeElapsed);
                statistics->setMoves(moveCounter);

                //update the statistics widget
                ui->statisticsWidget->updateStatistics();

                //render a frame
                qDebug() << "rendering frame" << frame+1 << "/" << numFrames << "at move" << moveNumber << "(elapsed time:" << QString::number((qreal)timer.elapsed()/1000,'f',3) << "seconds";
                qDebug() << "timeElapsed =" << timeElapsed;
                ui->graphicsView->getCubeGraphicsObject()->updateAll();
                render(&painter);

                //save the frame
                image.save("images/frame" + QString::number(frame+1) + ".png");
            }

            //apply the final moves
            while(moveNumber < reconstruction.length()){
                QPair<Move, qint64> pair = reconstruction.at(moveNumber);
                cube->move(pair.first);
                moveNumber++;
            }

            //update the statistics with the final time and movecount
            statistics->setTime(time);
            statistics->setMoves(moves);

            //update the statistics widget
            ui->statisticsWidget->updateStatistics();

            //re-enable the cube signals
            cube->blockSignals(false);

            //emit a cubeSolved signal, so the statisticsWidget will update
            //the timer labels stylesheet
            cube->cubeSolved();

            //render the last frame
            ui->graphicsView->getCubeGraphicsObject()->updateAll();
            render(&painter);

            //save the frame
            image.save("images/frame_end.png");
        }
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
    data["reconstruction"] = reconstruction.toJSON();
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
    reconstruction.fromJSON(data["reconstruction"].toObject());
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

    //read the file
    QFile f(name);
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

    //use middle mouse button for rotations
    if(button == Qt::MiddleButton) rotation = true;

    //if the solve is finished, allow rotations but not moves
    if(rotation){
        cube->rotate(axis, amount);
    }
    else{
        if(state == State::Finished){
            return;
        }

        if(settings->getMultislice()){
            cube->multisliceMove(axis, layer, amount);
        }
        else{
            cube->move(axis, layer, amount);
        }
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

void CubeWidget::onRotationDone(Axis axis, int amount){
    //if we are solving, add the rotation to the reconstruction
    if(state == State::Solving){
        qint64 time = statistics->getTime();
        reconstruction.addRotation(axis, amount, time);
    }
    //if we rotate during inspection, add the rotation with a time of 0
    else if(state == State::Inspecting){
        reconstruction.addRotation(axis, amount, 0);
    }
}

void CubeWidget::onCubeSolved(){
    //if we were solving, stop the timer
    if(state == State::Solving){
        statistics->stopTimer();
        state = State::Finished;
    }
}
