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
}

void Settings::setBackgroundColour(QColor c){
    backgroundColour = c;
}

void Settings::setLineColour(QColor c){
    lineColour = c;
}

void Settings::setLineWidth(int n){
    lineWidth = n;
}
