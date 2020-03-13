#ifndef REPLAYRECORDER_H
#define REPLAYRECORDER_H

#include <QImage>
#include <QObject>
#include <QPainter>
#include "cube.h"
#include "statistics.h"
#include "reconstruction.h"
#include "replayrecordersettings.h"

class CubeWidget;

class ReplayRecorder : public QObject
{
    Q_OBJECT
public:
    explicit ReplayRecorder(CubeWidget *cubeWidget, Reconstruction *reconstruction, Cube *cube, Statistics *statistics, QObject *parent = nullptr);

    void record();

    ReplayRecorderSettings *getSettings();

private:
    Cube *cube;
    CubeWidget *cubeWidget;
    Reconstruction *reconstruction;
    Statistics *statistics;

    ReplayRecorderSettings *settings;

signals:
    void frameRendered(int frame, int total);

};

#endif // REPLAYRECORDER_H
