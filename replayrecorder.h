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
    //Neutral: no render has started, or the previous render has finished
    //Recording: the replay recorder is currently rendering frames and
    //           sending them to ffmpeg
    //WaitingForFFmpeg: all of the frames have been rendered and sent to
    //                  ffmpeg, but we are waiting for ffmpeg to finish
    //                  processing the data
    enum State{
        Neutral,
        Recording,
        WaitingForFFmpeg
    };

    explicit ReplayRecorder(CubeWidget *cubeWidget, Reconstruction *reconstruction, Cube *cube, Statistics *statistics, QObject *parent = nullptr);

    void record();

    //if we are currently recording, this will schedule the recording to be
    //cancelled before the next frame is rendered
    void abort();

    ReplayRecorderSettings *getSettings();
    State getState();

private:
    Cube *cube;
    CubeWidget *cubeWidget;
    Reconstruction *reconstruction;
    Statistics *statistics;

    ReplayRecorderSettings *settings;

    FFmpegProcess *ffmpeg;

    State state;

    //we check this every frame to see whether we need to abort the render
    bool shouldAbort;

    void renderFrame(bool update = true, int numFrames = 1);

    void setState(State s);

private slots:
    void onFinished(int returnCode, QProcess::ExitStatus);

signals:
    void frameRendered(int frame, int total);
    void finished(int returnCode);
    void stateChanged();
    void aborted();

};

#endif // REPLAYRECORDER_H
