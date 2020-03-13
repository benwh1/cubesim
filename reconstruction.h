#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include "enums.h"
#include "move.h"

class Reconstruction
{
public:
    Reconstruction();

    void addMove(Axis axis, int layerStart, int layerEnd, int amount, qint64 time);
    void addRotation(Axis axis, int amount, qint64 time);

    Move getMove(int n);
    QPair<Move, qint64> at(int n);

    int length();
    qint64 totalTime();

    void reset();

    QString toString();
    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    //moves[i].second is the time in milliseconds since the start of the solve
    //when moves[i].first was done
    QList<QPair<Move, qint64>> moves;

};

#endif // RECONSTRUCTION_H
