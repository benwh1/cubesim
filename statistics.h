#ifndef STATISTICS_H
#define STATISTICS_H

#include <QDebug>
#include <QElapsedTimer>
#include <QObject>
#include <QTime>

class Statistics : public QObject
{
    Q_OBJECT
public:
    explicit Statistics(QObject *parent = nullptr);

    qint64 getTime();
    qint64 getMoves();
    int getTPS();

    void startTimer();
    void stopTimer();
    void reset();

    void doMove();

    bool timerRunning();

    QString timeString();
    QString movesString();
    QString tpsString();

private:
    QElapsedTimer timer;
    qint64 additionalTime;
    qint64 moves;

    //is the timer currently running?
    bool active;

signals:
    void timerStarted();
    void timerStopped();
    void timerReset();

};

#endif // STATISTICS_H
