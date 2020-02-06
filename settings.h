#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>
#include <QJsonObject>
#include <QObject>
#include "cube.h"

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
    QColor getColour(Cube::Face face);
    bool getMultislice();
    bool getGuideLinesCross();
    bool getGuideLinesPlus();
    bool getGuideLinesBox();
    QColor getGuideLineColour();
    int getGuideLineWidth();

    void setAntialiasing(bool b);
    void setBackgroundColour(QColor c);
    void setLineColour(QColor c);
    void setLineWidth(int n);
    void setColours(QList<QColor> c);
    void setColour(Cube::Face face, QColor c);
    void setMultislice(bool b);
    void setGuideLinesCross(bool b);
    void setGuideLinesPlus(bool b);
    void setGuideLinesBox(bool b);
    void setGuideLineColour(QColor c);
    void setGuideLineWidth(int n);

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

    //emitted when any setting is changed
    void settingChanged();

};

#endif // SETTINGS_H
