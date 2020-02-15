#ifndef MOVE_H
#define MOVE_H

#include <QString>
#include "enums.h"

class Move
{
public:
    //we use layerStart == 0 and layerEnd == -1 to denote a cube rotation
    Move(Axis axis, int layerStart, int layerEnd, int amount);

    QString toString();

private:
    Axis axis;
    int layerStart;
    int layerEnd;
    int amount;

};

#endif // MOVE_H
