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

    void setAntialiasing(bool b);
    void setBackgroundColour(QColor c);
    void setLineColour(QColor c);
    void setLineWidth(int n);

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    QColor backgroundColour;
    QColor lineColour;
    bool antialiasing;
    int lineWidth;

signals:
    void antialiasingChanged();
    void backgroundColourChanged();
    void lineColourChanged();
    void lineWidthChanged();

};

#endif // SETTINGS_H
