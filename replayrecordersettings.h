#ifndef REPLAYRECORDERSETTINGS_H
#define REPLAYRECORDERSETTINGS_H

#include <QObject>
#include "reconstruction.h"

class ReplayRecorderSettings : public QObject
{
    Q_OBJECT
public:
    explicit ReplayRecorderSettings(Reconstruction *reconstruction, QObject *parent = nullptr);

    int getPlaybackFrameRate();
    qreal getSpeed();
    int getNumberOfFrames();
    qreal getTimePerFrame();

    void setPlaybackFrameRate(int n);

    //the following functions change speed, numberOfFrames, timePerFrame
    void setSpeed(qreal r);
    void setNumberOfFrames(int n);
    void setTimePerFrame(qreal r);

private:
    Reconstruction *reconstruction;

    int playbackFrameRate;
    qreal speed;
    int numberOfFrames;
    qreal timePerFrame;

signals:
    void settingChanged();

};

#endif // REPLAYRECORDERSETTINGS_H
