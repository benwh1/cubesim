#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include <QList>
#include "enums.h"
#include "move.h"

class Reconstruction
{
public:
    Reconstruction();

    //here, time is the total time from the start of the solve, not the
    //amount of time since the previous move
    void addMove(Axis axis, int layerStart, int layerEnd, int amount, qint64 time);
    void addRotation(Axis axis, int amount, qint64 time);

    void reset();
    QString toString();

private:
    //moves[i].second is the time in milliseconds between moves[i-1] and moves[i]
    QList<QPair<Move, qint64>> moves;

};

#endif // RECONSTRUCTION_H
