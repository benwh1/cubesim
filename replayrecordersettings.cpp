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

    //set the default values
    playbackFrameRate = 30;
    extremeFrameDuration = 2;
    videoSize = QSize(800, 600);
    //this will also compute numberOfFrames, timePerFrame, videoLength
    setSpeed(1);

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

qreal ReplayRecorderSettings::getExtremeFrameDuration(){
    return extremeFrameDuration;
}

qreal ReplayRecorderSettings::getVideoLength(){
    return videoLength;
}

int ReplayRecorderSettings::getVideoWidth(){
    return videoSize.width();
}

int ReplayRecorderSettings::getVideoHeight(){
    return videoSize.height();
}

QSize ReplayRecorderSettings::getVideoSize(){
    return videoSize;
}

void ReplayRecorderSettings::setPlaybackFrameRate(int n){
    playbackFrameRate = n;
    timePerFrame = speed/playbackFrameRate;
    numberOfFrames = reconstruction->totalTime()/(1000*timePerFrame) + 2;
    videoLength = (qreal)(numberOfFrames - 2)/playbackFrameRate + extremeFrameDuration * 2;

    emit settingChanged();
}

void ReplayRecorderSettings::setSpeed(qreal r){
    speed = r;
    timePerFrame = speed/playbackFrameRate;
    numberOfFrames = reconstruction->totalTime()/(1000*timePerFrame) + 2;
    videoLength = (qreal)(numberOfFrames - 2)/playbackFrameRate + extremeFrameDuration * 2;

    emit settingChanged();
}

void ReplayRecorderSettings::setNumberOfFrames(int n){
    numberOfFrames = n;
    timePerFrame = (qreal)reconstruction->totalTime()/(1000*(numberOfFrames - 2));
    speed = timePerFrame*playbackFrameRate;
    videoLength = (qreal)(numberOfFrames - 2)/playbackFrameRate + extremeFrameDuration * 2;

    emit settingChanged();
}

void ReplayRecorderSettings::setTimePerFrame(qreal r){
    timePerFrame = r;
    speed = timePerFrame*playbackFrameRate;
    numberOfFrames = reconstruction->totalTime()/(1000*timePerFrame) + 2;
    videoLength = (qreal)(numberOfFrames - 2)/playbackFrameRate + extremeFrameDuration * 2;

    emit settingChanged();
}

void ReplayRecorderSettings::setExtremeFrameDuration(qreal r){
    extremeFrameDuration = r;
    videoLength = (qreal)(numberOfFrames - 2)/playbackFrameRate + extremeFrameDuration * 2;
    timePerFrame = speed/playbackFrameRate;
    numberOfFrames = reconstruction->totalTime()/(1000*timePerFrame) + 2;

    emit settingChanged();
}

void ReplayRecorderSettings::setVideoLength(qreal r){
    videoLength = r;
    numberOfFrames = (videoLength - extremeFrameDuration * 2) * playbackFrameRate + 2;
    timePerFrame = (qreal)reconstruction->totalTime()/(1000*(numberOfFrames - 2));
    speed = timePerFrame*playbackFrameRate;

    emit settingChanged();
}

void ReplayRecorderSettings::setVideoWidth(int n){
    videoSize.setWidth(n);

    emit settingChanged();
}

void ReplayRecorderSettings::setVideoHeight(int n){
    videoSize.setHeight(n);

    emit settingChanged();
}
