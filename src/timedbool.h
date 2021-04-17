#ifndef TIMEDBOOL_H
#define TIMEDBOOL_H

#include <QObject>
#include <QTimer>

class TimedBool : public QObject
{
    Q_OBJECT
public:
    explicit TimedBool(bool defaultValue = false, int duration = 1000, QObject *parent = nullptr);

    void setDuration(int d);

    TimedBool& operator=(const bool b);
    bool operator==(const bool b) const;
    bool operator!() const;

private:
    bool value;
    bool defaultValue;

    QTimer timer;

private slots:
    void onTimeout();

};

#endif // TIMEDBOOL_H
