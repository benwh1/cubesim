#ifndef REPLAYRECORDER_H
#define REPLAYRECORDER_H

#include <QImage>
#include <QPainter>
#include "cube.h"
#include "statistics.h"
#include "reconstruction.h"

class CubeWidget;

class ReplayRecorder
{
public:
    ReplayRecorder(CubeWidget *cubeWidget, Reconstruction *reconstruction, Cube *cube, Statistics *statistics);

    void record(int frameRate, qreal speed);

private:
    Cube *cube;
    CubeWidget *cubeWidget;
    Reconstruction *reconstruction;
    Statistics *statistics;

};

#endif // REPLAYRECORDER_H
