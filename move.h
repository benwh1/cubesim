#ifndef MOVE_H
#define MOVE_H

#include <QString>
#include "enums.h"

class Move
{
public:
    Move(Axis axis, int layerStart, int layerEnd, int amount);

    QString toString();

private:
    Axis axis;
    int layerStart;
    int layerEnd;
    int amount;

};

#endif // MOVE_H
