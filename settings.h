#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>
#include <QJsonObject>
#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    bool getAntialiasing();
    QColor getBackgroundColour();
    QColor getLineColour();
    int getLineWidth();
    bool getMultislice();
    bool getGuideLines();

    void setAntialiasing(bool b);
    void setBackgroundColour(QColor c);
    void setLineColour(QColor c);
    void setLineWidth(int n);
    void setMultislice(bool b);
    void setGuideLines(bool b);

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    QColor backgroundColour;
    QColor lineColour;
    bool antialiasing;
    int lineWidth;
    bool multislice;
    bool guideLines;

signals:
    void antialiasingChanged();
    void backgroundColourChanged();
    void lineColourChanged();
    void lineWidthChanged();
    void multisliceChanged();
    void guideLinesChanged();

    //emitted when any setting is changed
    void settingChanged();

};

#endif // SETTINGS_H
