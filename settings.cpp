#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    antialiasing = false;
    backgroundColour = QColor(255, 228, 196);
    lineColour = Qt::black;
    lineWidth = 0;
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

void Settings::setAntialiasing(bool b){
    antialiasing = b;
    emit antialiasingChanged();
}

void Settings::setBackgroundColour(QColor c){
    backgroundColour = c;
    emit backgroundColourChanged();
}

void Settings::setLineColour(QColor c){
    lineColour = c;
    emit lineColourChanged();
}

void Settings::setLineWidth(int n){
    lineWidth = n;
    emit lineWidthChanged();
}

QJsonObject Settings::toJSON(){
    QJsonObject data;

    data["antialiasing"] = antialiasing;
    data["backgroundColour"] = backgroundColour.name(QColor::HexArgb);
    data["lineColour"] = lineColour.name(QColor::HexArgb);
    data["lineWidth"] = lineWidth;

    return data;
}

void Settings::fromJSON(QJsonObject data){
    setAntialiasing(data["antialiasing"].toBool());
    setBackgroundColour(QColor(data["backgroundColour"].toString()));
    setLineColour(QColor(data["lineColour"].toString()));
    setLineWidth(data["lineWidth"].toInt());
}
