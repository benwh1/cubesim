#ifndef REPLAYRECORDERSETTINGS_H
#define REPLAYRECORDERSETTINGS_H

#include <QObject>
#include "reconstruction.h"

class ReplayRecorderSettings : public QObject
{
    Q_OBJECT
public:
    explicit ReplayRecorderSettings(Reconstruction *reconstruction, QObject *parent = nullptr);

    void reset();

    int getPlaybackFrameRate();
    qreal getSpeed();
    int getNumberOfFrames();
    qreal getTimePerFrame();
    qreal getExtremeFrameDuration();
    qreal getVideoLength();

    void setPlaybackFrameRate(int n);
    //the following functions change speed, numberOfFrames, timePerFrame
    void setSpeed(qreal r);
    void setNumberOfFrames(int n);
    void setTimePerFrame(qreal r);
    void setExtremeFrameDuration(qreal r);
    void setVideoLength(qreal r);

private:
    Reconstruction *reconstruction;

    int playbackFrameRate;
    qreal speed;
    int numberOfFrames;
    qreal timePerFrame;
    qreal extremeFrameDuration;
    qreal videoLength;

signals:
    void settingChanged();

};

#endif // REPLAYRECORDERSETTINGS_H
