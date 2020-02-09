#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include "enums.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    bool getAntialiasing();
    QColor getBackgroundColour();
    QColor getLineColour();
    int getLineWidth();
    QList<QColor> getColours();
    QColor getColour(Face face);
    bool getMultislice();
    bool getGuideLinesCross();
    bool getGuideLinesPlus();
    bool getGuideLinesBox();
    QColor getGuideLineColour();
    int getGuideLineWidth();
    bool getSupercube();
    QString getSupercubeStickers();
    qreal getPochmannBarThickness();
    bool getPochmannCage();

    void setAntialiasing(bool b);
    void setBackgroundColour(QColor c);
    void setLineColour(QColor c);
    void setLineWidth(int n);
    void setColours(QList<QColor> c);
    void setColour(Face face, QColor c);
    void setMultislice(bool b);
    void setGuideLinesCross(bool b);
    void setGuideLinesPlus(bool b);
    void setGuideLinesBox(bool b);
    void setGuideLineColour(QColor c);
    void setGuideLineWidth(int n);
    void setSupercube(bool b);
    void setSupercubeStickers(QString s);
    void setPochmannBarThickness(qreal r);
    void setPochmannCage(bool b);

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    QColor backgroundColour;
    QColor lineColour;
    bool antialiasing;
    int lineWidth;
    QList<QColor> colours;
    bool multislice;
    bool guideLinesCross;
    bool guideLinesPlus;
    bool guideLinesBox;
    QColor guideLineColour;
    int guideLineWidth;
    bool supercube;
    QString supercubeStickers;
    qreal pochmannBarThickness;
    bool pochmannCage;

signals:
    void antialiasingChanged();
    void backgroundColourChanged();
    void lineColourChanged();
    void lineWidthChanged();
    void coloursChanged();
    void multisliceChanged();
    void guideLinesCrossChanged();
    void guideLinesPlusChanged();
    void guideLinesBoxChanged();
    void guideLineColourChanged();
    void guideLineWidthChanged();
    void supercubeChanged();
    void supercubeStickersChanged();
    void pochmannBarThicknessChanged();
    void pochmannCageChanged();

    //emitted when any setting is changed
    void settingChanged();

};

#endif // SETTINGS_H
