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
    pochmannBarThickness = 0.2;
    pochmannCage = true;

    //parent should be the cube widget, so try and cast to QWidget
    //if it fails for whatever reason, then p will be nullptr
    QWidget *p = qobject_cast<QWidget*>(parent);
    controls = new Controls(p);
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

qreal Settings::getPochmannBarThickness(){
    return pochmannBarThickness;
}

bool Settings::getPochmannCage(){
    return pochmannCage;
}

Controls *Settings::getControls(){
    return controls;
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

void Settings::setPochmannBarThickness(qreal r){
    pochmannBarThickness = r;
    emit pochmannBarThicknessChanged();
    emit settingChanged();
}

void Settings::setPochmannCage(bool b){
    pochmannCage = b;;
    emit pochmannCageChanged();
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
    data["pochmannBarThickness"] = pochmannBarThickness;
    data["pochmannCage"] = pochmannCage;

    data["controls"] = controls->toJSON();

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
    setPochmannBarThickness(data["pochmannBarThickness"].toDouble());
    setPochmannCage(data["pochmannCage"].toBool());

    controls->fromJSON(data["controls"].toObject());
}

void Settings::save(QString filename){
    QJsonObject data = toJSON();
    QJsonDocument document(data);

    QFile f(filename);
    f.open(QFile::WriteOnly);
    f.write(document.toBinaryData());
    f.close();
}

void Settings::load(QString filename){
    QFile f(filename);
    if(f.exists()){
        f.open(QFile::ReadOnly);
        QJsonDocument document = QJsonDocument::fromBinaryData(f.readAll());
        f.close();

        QJsonObject data = document.object();
        fromJSON(data);
    }
}
