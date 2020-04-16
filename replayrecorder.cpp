#include "replayrecorder.h"

#include "cubewidget.h"

ReplayRecorder::ReplayRecorder(CubeWidget *cubeWidget, Reconstruction *reconstruction, Cube *cube, Statistics *statistics, QObject *parent) :
    QObject(parent)
{
    this->cube = cube;
    this->cubeWidget = cubeWidget;
    this->reconstruction = reconstruction;
    this->statistics = statistics;

    settings = new ReplayRecorderSettings(reconstruction, this);
    ffmpeg = new FFmpegProcess(this);

    //propogate the finished signal
    connect(ffmpeg, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int, QProcess::ExitStatus)));
}

void ReplayRecorder::record(){
    //make the videos directory if it doesn't already exist
    QDir::current().mkdir("videos");

    //set the arguments of the ffmpeg process:
    //- set the playback framerate
    //- tell ffmpeg that the data comes from a pipe, not a file
    //- use the "copy" codec, so we don't re-encode the video
    //- set the output file
    QStringList args;
    args << "-framerate" << QString::number(settings->getPlaybackFrameRate())
         << "-i" << "-"
         << "-c" << "copy"
         << "videos/out.mkv";
    ffmpeg->setArguments(args);

    //start ffmpeg
    ffmpeg->start();

    //store the current size of the widget so it can be restored later
    QSize oldSize = cubeWidget->size();

    //resize the widget
    cubeWidget->resize(settings->getVideoSize());

    //force the resize event to be processed immediately. if we don't do
    //this, then the first frame of the video may show scroll bars when it
    //shouldn't, because the resize event didn't get processed before the
    //first frame is rendered.
    QApplication::processEvents();

    //store the final time and movecount for use in the last frame
    qint64 time = statistics->getTime();
    qint64 moves = statistics->getMoves();

    //length of the reconstruction
    int length = reconstruction->length();

    //reset the statistics for the first frame
    //this emits a signal which causes the statisticsWidget to clear
    statistics->reset();

    //block all signals from the cube so that the CubeGraphicsObject
    //doesn't try to update after every single move
    cube->blockSignals(true);

    //read the settings
    int numFrames = settings->getNumberOfFrames();
    qreal msPerFrame = 1000*settings->getTimePerFrame();

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
        Move move = reconstruction->getMove(moveNumber);
        if(!move.isRotation()) break;
        cube->move(move);
        moveNumber++;
    }

    //calculate how many frames the start and end frames should be
    int numExtremeFrames = qRound(settings->getExtremeFrameDuration() *
                                  settings->getPlaybackFrameRate());

    //render the first frame
    renderFrame(true, numExtremeFrames);
    emit frameRendered(1, numFrames);

    //render the solving frames
    for(int frame=2; frame<=numFrames-1; frame++){
        //how much time has passed so far?
        qint64 timeElapsed = (frame-1) * msPerFrame;

        //do moves until we reach the current time
        bool movesApplied = false;
        QPair<Move, qint64> pair;
        while(true){
            //check if we're at the end of the reconstruction
            if(moveNumber >= length) break;

            //get the next move, and check if we are still within
            //the next frame to be rendered
            pair = reconstruction->at(moveNumber);
            if(pair.second > timeElapsed) break;

            //check the move after the next move, and see if it is the
            //inverse of the next move and that both moves are within
            //the current frame. if so, skip over both moves
            bool skipTwoMoves;
            if(moveNumber+1 < length){
                QPair<Move, qint64> pair2 = reconstruction->at(moveNumber+1);

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

                continue;
            }

            //apply the move
            cube->move(pair.first);
            movesApplied = true;

            //increment the move counter and the non-rotation counter
            moveNumber++;
            if(!pair.first.isRotation()){
                moveCounter++;
            }
        }

        //update the statistics
        statistics->setTime(timeElapsed);
        statistics->setMoves(moveCounter);

        //render a frame but only repaint the cube if any moves have been done
        renderFrame(movesApplied);
        emit frameRendered(frame, numFrames);
    }

    //apply the final moves
    while(moveNumber < length){
        QPair<Move, qint64> pair = reconstruction->at(moveNumber);
        cube->move(pair.first);
        moveNumber++;
    }

    //update the statistics with the final time and movecount
    statistics->setTime(time);
    statistics->setMoves(moves);

    //update the cubeGraphicsObject and emit a cubeSolved signal, so that
    //the statisticsWidget will update the timer labels stylesheet
    cube->blockSignals(false);
    cube->cubeStateChanged();
    cube->cubeSolved();
    cube->blockSignals(true);

    //render the last frame
    renderFrame(false, numExtremeFrames);
    emit frameRendered(numFrames, numFrames);

    //schedule stdin to be closed once all of the data has been written.
    //this tells ffmpeg to write the last part of the file, then exit
    ffmpeg->closeWriteChannel();

    //re-enable the cube signals
    cube->blockSignals(false);

    //resize the widget to the old size
    cubeWidget->resize(oldSize);
}

ReplayRecorderSettings *ReplayRecorder::getSettings(){
    return settings;
}

void ReplayRecorder::renderFrame(bool update, int numFrames){
    //repaint the cubeWidget if necessary
    if(update){
        //temporarily unblock the cube signals so we can call cubeStateChanged
        //which will cause the cubeGraphicsObject to run updateAll
        cube->blockSignals(false);
        cube->cubeStateChanged();
        cube->blockSignals(true);
    }

    //construct an image and a painter to draw on the image
    QImage image(cubeWidget->size(), QImage::Format_ARGB32);
    QPainter painter(&image);

    //render the cubeWidget to the image
    cubeWidget->render(&painter);

    //send the frame to ffmpeg
    ffmpeg->writeFrame(image, numFrames);
}

void ReplayRecorder::onFinished(int returnCode, QProcess::ExitStatus){
    emit finished(returnCode);
}
