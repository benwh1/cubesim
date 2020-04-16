#ifndef REPLAYRECORDER_H
#define REPLAYRECORDER_H

#include <QApplication>
#include <QImage>
#include <QObject>
#include <QPainter>
#include "ffmpegprocess.h"
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

    FFmpegProcess *ffmpeg;

    void renderFrame(bool update = true, int numFrames = 1);

private slots:
    void onFinished(int returnCode, QProcess::ExitStatus);

signals:
    void frameRendered(int frame, int total);

    void finished(int returnCode);

};

#endif // REPLAYRECORDER_H
