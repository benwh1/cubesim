#include "move.h"

Move::Move(Axis axis, int layerStart, int layerEnd, int amount)
{
    this->axis = axis;
    this->layerStart = layerStart;
    this->layerEnd = layerEnd;
    this->amount = amount;
}
