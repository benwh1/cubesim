#ifndef REPLAYRECORDERSETTINGS_H
#define REPLAYRECORDERSETTINGS_H

#include <QObject>
#include <QSize>
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
    int getVideoWidth();
    int getVideoHeight();
    QSize getVideoSize();

    /* list of dependencies A -> B where changing A will cause B to be
     * recomputed directly:
     *
     * playbackFrameRate -> videoLength
     * speed -> timePerFrame
     * speed -> numberOfFrames
     * numberOfFrames -> speed
     * numberOfFrames -> timePerFrame
     * timePerFrame -> speed
     * timePerFrame -> numberOfFrames
     * extremeFrameDuration -> videoLength
     * videoLength -> speed
     * videoLength -> numberOfFrames
     * videoLength -> timePerFrame
     */
    void setPlaybackFrameRate(int n);
    void setSpeed(qreal r);
    void setNumberOfFrames(int n);
    void setTimePerFrame(qreal r);
    void setExtremeFrameDuration(qreal r);
    void setVideoLength(qreal r);
    void setVideoWidth(int n);
    void setVideoHeight(int n);

private:
    Reconstruction *reconstruction;

    int playbackFrameRate;
    qreal speed;
    int numberOfFrames;
    qreal timePerFrame;
    qreal extremeFrameDuration;
    qreal videoLength;
    QSize videoSize;

signals:
    void settingChanged();

};

#endif // REPLAYRECORDERSETTINGS_H
