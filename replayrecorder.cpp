#include "replayrecorder.h"

#include "cubewidget.h"

ReplayRecorder::ReplayRecorder(CubeWidget *cubeWidget, Reconstruction *reconstruction, Cube *cube, Statistics *statistics)
{
    this->cube = cube;
    this->cubeWidget = cubeWidget;
    this->reconstruction = reconstruction;
    this->statistics = statistics;
}

void ReplayRecorder::record(int frameRate, qreal speed){
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

    //create an image and a painter
    QImage image(cubeWidget->size(), QImage::Format_ARGB32);
    QPainter painter(&image);

    //calculate the number of frames in the video
    qreal msPerFrame = 1000.0*speed/frameRate;
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
        Move move = reconstruction->getMove(moveNumber);
        if(!move.isRotation()) break;
        cube->move(move);
        moveNumber++;
    }

    //temporarily unblock the cube signals so we can call cubeStateChanged
    //which will cause the cubeGraphicsObject to run updateAll
    cube->blockSignals(false);
    cube->cubeStateChanged();
    cube->blockSignals(true);

    //render the first frame
    cubeWidget->render(&painter);
    image.save("images/frame_start.png");

    //render the frames
    for(int frame=0; frame<numFrames; frame++){
        //how much time has passed so far?
        qint64 timeElapsed = frame * msPerFrame;

        //do moves until we reach the current time
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

            //increment the move counter and the non-rotation counter
            moveNumber++;
            if(!pair.first.isRotation()){
                moveCounter++;
            }
        }

        //update the statistics
        statistics->setTime(timeElapsed);
        statistics->setMoves(moveCounter);

        //update the cubeGraphicsObject in the same way as before
        cube->blockSignals(false);
        cube->cubeStateChanged();
        cube->blockSignals(true);

        qDebug() << "rendering frame" << frame+1 << "/" << numFrames << "at move" << moveNumber << "(elapsed time:" << QString::number((qreal)timer.elapsed()/1000,'f',3) << "seconds";
        qDebug() << "timeElapsed =" << timeElapsed;

        //render a frame
        cubeWidget->render(&painter);

        //save the frame
        image.save("images/frame" + QString::number(frame+1) + ".png");
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

    //emit a cubeSolved signal, so the statisticsWidget will update
    //the timer labels stylesheet
    cube->cubeSolved();

    //update the cubeGraphicsObject as before
    cube->blockSignals(false);
    cube->cubeStateChanged();
    cube->blockSignals(true);

    //render the last frame
    cubeWidget->render(&painter);

    //save the frame
    image.save("images/frame_end.png");

    //re-enable the cube signals
    cube->blockSignals(false);
}
