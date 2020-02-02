#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    antialiasing = false;
    backgroundColour = QColor(255, 228, 196);
    lineColour = Qt::black;
    lineWidth = 0;
    multislice = false;
    guideLinesCross = false;
}

bool Settings::getAntialiasing(){
    return antialiasing;
}

QColor Settings::getBackgroundColour(){
    return backgroundColour;
}

QColor Settings::getLineColour(){
    return lineColour;
}

int Settings::getLineWidth(){
    return lineWidth;
}

bool Settings::getMultislice(){
    return multislice;
}

bool Settings::getGuideLinesCross(){
    return guideLinesCross;
}

void Settings::setAntialiasing(bool b){
    antialiasing = b;
    emit antialiasingChanged();
    emit settingChanged();
}

void Settings::setBackgroundColour(QColor c){
    backgroundColour = c;
    emit backgroundColourChanged();
    emit settingChanged();
}

void Settings::setLineColour(QColor c){
    lineColour = c;
    emit lineColourChanged();
    emit settingChanged();
}

void Settings::setLineWidth(int n){
    lineWidth = n;
    emit lineWidthChanged();
    emit settingChanged();
}

void Settings::setMultislice(bool b){
    multislice = b;
    emit multisliceChanged();
    emit settingChanged();
}

void Settings::setGuideLinesCross(bool b){
    guideLinesCross = b;
    emit guideLinesCrossChanged();
    emit settingChanged();
}

QJsonObject Settings::toJSON(){
    QJsonObject data;

    data["antialiasing"] = antialiasing;
    data["backgroundColour"] = backgroundColour.name(QColor::HexArgb);
    data["lineColour"] = lineColour.name(QColor::HexArgb);
    data["lineWidth"] = lineWidth;
    data["multislice"] = multislice;
    data["guideLinesCross"] = guideLinesCross;

    return data;
}

void Settings::fromJSON(QJsonObject data){
    setAntialiasing(data["antialiasing"].toBool());
    setBackgroundColour(QColor(data["backgroundColour"].toString()));
    setLineColour(QColor(data["lineColour"].toString()));
    setLineWidth(data["lineWidth"].toInt());
    setMultislice(data["multislice"].toBool());
    setGuideLinesCross(data["guideLinesCross"].toBool());
}
