#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include "cube.h"
#include "statistics.h"

class Reconstruction : public QObject
{
    Q_OBJECT
public:
    explicit Reconstruction(Cube *cube, Statistics *statistics, QObject *parent = nullptr);

    Move getMove(int n);
    QPair<Move, qint64> at(int n);

    int length();
    qint64 totalTime();

    int numRotations();

    void start();
    void finish();
    void reset();

    QString toString();
    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    //moves[i].second is the time in milliseconds since the start of the solve
    //when moves[i].first was done
    QList<QPair<Move, qint64>> moves;

    //are we currently recording moves?
    bool active;

    Cube *cube;
    Statistics *statistics;

    void addMove(Move move, qint64 time);
    void addRotation(Move move, qint64 time);

private slots:
    void onMoveDone(Move move);
    void onRotationDone(Move move);

};

#endif // RECONSTRUCTION_H
