#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    antialiasing = false;
    backgroundColour = QColor(255, 228, 196);
    lineColour = Qt::black;
    lineWidth = 0;
    colours << QColor(Qt::white)
            << QColor(Qt::green)
            << QColor(Qt::red)
            << QColor(Qt::blue)
            << QColor(255,165,0)
            << QColor(Qt::yellow);
    multislice = false;
    guideLinesCross = false;
    guideLinesPlus = false;
    guideLinesBox = false;
    guideLineColour = Qt::black;
    guideLineWidth = 2;
    supercube = false;
    supercubeStickers = "Arrows";
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

QList<QColor> Settings::getColours(){
    return colours;
}

QColor Settings::getColour(Face face){
    return colours[face];
}

bool Settings::getMultislice(){
    return multislice;
}

bool Settings::getGuideLinesCross(){
    return guideLinesCross;
}

bool Settings::getGuideLinesPlus(){
    return guideLinesPlus;
}

bool Settings::getGuideLinesBox(){
    return guideLinesBox;
}

QColor Settings::getGuideLineColour(){
    return guideLineColour;
}

int Settings::getGuideLineWidth(){
    return guideLineWidth;
}

bool Settings::getSupercube(){
    return supercube;
}

QString Settings::getSupercubeStickers(){
    return supercubeStickers;
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

void Settings::setColours(QList<QColor> c){
    colours = c;
    emit coloursChanged();
    emit settingChanged();
}

void Settings::setColour(Face face, QColor c){
    colours[face] = c;
    emit coloursChanged();
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

void Settings::setGuideLinesPlus(bool b){
    guideLinesPlus = b;
    emit guideLinesPlusChanged();
    emit settingChanged();
}

void Settings::setGuideLinesBox(bool b){
    guideLinesBox = b;
    emit guideLinesBoxChanged();
    emit settingChanged();
}

void Settings::setGuideLineColour(QColor c){
    guideLineColour = c;
    emit guideLineColourChanged();
    emit settingChanged();
}

void Settings::setGuideLineWidth(int n){
    guideLineWidth = n;
    emit guideLineWidthChanged();
    emit settingChanged();
}

void Settings::setSupercube(bool b){
    supercube = b;
    emit supercubeChanged();
    emit settingChanged();
}

void Settings::setSupercubeStickers(QString s){
    supercubeStickers = s;
    emit supercubeStickersChanged();
    emit settingChanged();
}

QJsonObject Settings::toJSON(){
    QJsonObject data;

    data["antialiasing"] = antialiasing;
    data["backgroundColour"] = backgroundColour.name(QColor::HexArgb);
    data["lineColour"] = lineColour.name(QColor::HexArgb);
    data["lineWidth"] = lineWidth;

    QJsonArray coloursArray;
    for(int i=0; i<6; i++){
        coloursArray.append(colours[i].name(QColor::HexArgb));
    }
    data["colours"] = coloursArray;

    data["multislice"] = multislice;
    data["guideLinesCross"] = guideLinesCross;
    data["guideLinesPlus"] = guideLinesPlus;
    data["guideLinesBox"] = guideLinesBox;
    data["guideLineColour"] = guideLineColour.name(QColor::HexArgb);
    data["guideLineWidth"] = guideLineWidth;
    data["supercube"] = supercube;
    data["supercubeStickers"] = supercubeStickers;

    return data;
}

void Settings::fromJSON(QJsonObject data){
    setAntialiasing(data["antialiasing"].toBool());
    setBackgroundColour(QColor(data["backgroundColour"].toString()));
    setLineColour(QColor(data["lineColour"].toString()));
    setLineWidth(data["lineWidth"].toInt());

    QJsonArray coloursArray = data["colours"].toArray();
    QList<QColor> c;
    for(int i=0; i<6; i++){
        c.append(QColor(coloursArray[i].toString()));
    }
    setColours(c);

    setMultislice(data["multislice"].toBool());
    setGuideLinesCross(data["guideLinesCross"].toBool());
    setGuideLinesPlus(data["guideLinesPlus"].toBool());
    setGuideLinesBox(data["guideLinesBox"].toBool());
    setGuideLineColour(QColor(data["guideLineColour"].toString()));
    setGuideLineWidth(data["guideLineWidth"].toInt());
    setSupercube(data["supercube"].toBool());
    setSupercubeStickers(data["supercubeStickers"].toString());
}
