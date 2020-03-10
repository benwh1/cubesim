#ifndef MOVE_H
#define MOVE_H

#include <QJsonObject>
#include <QString>
#include "enums.h"

class Move
{
public:
    Move();

    //we use layerStart == 0 and layerEnd == -1 to denote a cube rotation
    Move(Axis axis, int layerStart, int layerEnd, int amount);

    bool isRotation();

    Axis getAxis();
    int getLayerStart();
    int getLayerEnd();
    int getAmount();

    QString toString();
    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    Axis axis;
    int layerStart;
    int layerEnd;
    int amount;

};

#endif // MOVE_H
