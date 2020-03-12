#ifndef REPLAYRECORDER_H
#define REPLAYRECORDER_H

#include <QImage>
#include <QPainter>
#include "cube.h"
#include "cubewidget.h"
#include "statistics.h"

class ReplayRecorder
{
public:
    ReplayRecorder(CubeWidget *cubeWidget, Cube *cube, Statistics *statistics);

    void record(Reconstruction *reconstruction, int frameRate, qreal speed);

private:
    CubeWidget *cubeWidget;
    Cube *cube;
    Statistics *statistics;

};

#endif // REPLAYRECORDER_H
