#ifndef STATISTICS_H
#define STATISTICS_H

#include <QDebug>
#include <QElapsedTimer>
#include <QJsonObject>
#include <QObject>
#include <QTime>
#include "cube.h"
#include "formatting.h"

class Statistics : public QObject
{
    Q_OBJECT
public:
    explicit Statistics(Cube *cube, QObject *parent = nullptr);

    qint64 getTime();
    qint64 getMoves();
    int getTPS();

    int getCubeSize();
    int getNumberOfPieces();

    //these functions are all under the assumption that the solve is finished
    qint64 getPiecesPerSecond();
    qint64 getTimePerPiece();
    qint64 getPiecesPerMove();
    qint64 getMovesPerPiece();

    void setTime(qint64 time);
    void setMoves(qint64 moves);

    void startTimer();
    void stopTimer();
    void reset();

    void doMove();

    bool timerRunning();

    QString timeString();
    QString movesString();
    QString tpsString();
    QString cubeSizeString();
    QString numberOfPiecesString();
    QString piecesPerSecondString();
    QString timePerPieceString();
    QString piecesPerMoveString();
    QString movesPerPieceString();

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    QElapsedTimer timer;
    qint64 additionalTime;
    qint64 moves;

    Cube *cube;

    //is the timer currently running?
    bool active;

signals:
    void timerStarted();
    void timerStopped();
    void timerReset();

    //emitted whenever the movecount or time is changed
    //not emitted by the timer updated when the timer is running, because it
    //changes continuously
    void statisticChanged();

    void cubeSizeChanged();

};

#endif // STATISTICS_H
