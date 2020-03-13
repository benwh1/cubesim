#include "replayrecordersettings.h"

ReplayRecorderSettings::ReplayRecorderSettings(Reconstruction *reconstruction, QObject *parent) :
    QObject(parent)
{
    this->reconstruction = reconstruction;

    playbackFrameRate = 30;
    speed = 1;
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

    //get the total solve time
    int length = reconstruction->length();
    qint64 time = reconstruction->at(length-1).second;

    //number of frames for the solving part is time/timePerFrame
    //then we have the first frame and the last frame
    numberOfFrames = time/timePerFrame + 2;

    emit settingChanged();
}

void ReplayRecorderSettings::setNumberOfFrames(int n){
    numberOfFrames = n;

    //get the total solve time
    int length = reconstruction->length();
    qint64 time = reconstruction->at(length-1).second;

    timePerFrame = time/(n-2);
    speed = timePerFrame*playbackFrameRate/1000;

    emit settingChanged();
}

void ReplayRecorderSettings::setTimePerFrame(qreal r){
    timePerFrame = r;
    speed = timePerFrame*playbackFrameRate/1000;

    //get the total solve time
    int length = reconstruction->length();
    qint64 time = reconstruction->at(length-1).second;

    numberOfFrames = time/timePerFrame + 2;

    emit settingChanged();
}
