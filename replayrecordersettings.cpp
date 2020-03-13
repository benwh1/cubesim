#include "replayrecordersettings.h"

ReplayRecorderSettings::ReplayRecorderSettings(Reconstruction *reconstruction, QObject *parent) :
    QObject(parent)
{
    this->reconstruction = reconstruction;

    reset();
}

void ReplayRecorderSettings::reset(){
    //we only need to emit settingChanged once, so block signals for now
    blockSignals(true);

    setPlaybackFrameRate(30);
    setSpeed(1); //will also set timePerFrame = 33.333ms and numberOfFrames = 2

    blockSignals(false);

    emit settingChanged();
}

int ReplayRecorderSettings::getPlaybackFrameRate(){
    return playbackFrameRate;
}

qreal ReplayRecorderSettings::getSpeed(){
    return speed;
}

int ReplayRecorderSettings::getNumberOfFrames(){
    return numberOfFrames;
}

qreal ReplayRecorderSettings::getTimePerFrame(){
    return timePerFrame;
}

void ReplayRecorderSettings::setPlaybackFrameRate(int n){
    playbackFrameRate = n;

    emit settingChanged();
}

void ReplayRecorderSettings::setSpeed(qreal r){
    speed = r;
    timePerFrame = (1000*speed)/playbackFrameRate;
    numberOfFrames = reconstruction->totalTime()/timePerFrame + 2;

    emit settingChanged();
}

void ReplayRecorderSettings::setNumberOfFrames(int n){
    numberOfFrames = n;
    timePerFrame = (qreal)reconstruction->totalTime()/(numberOfFrames - 2);
    speed = timePerFrame*playbackFrameRate/1000;

    emit settingChanged();
}

void ReplayRecorderSettings::setTimePerFrame(qreal r){
    timePerFrame = r;
    speed = timePerFrame*playbackFrameRate/1000;
    numberOfFrames = reconstruction->totalTime()/timePerFrame + 2;

    emit settingChanged();
}
