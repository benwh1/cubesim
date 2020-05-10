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

    Move inverse();

    //check if this move followed by m can be simplified (e.g. U U2)
    bool cancelsWith(Move m);

    //if cancelsWith(m) is true, return the simplified move
    //otherwise return a default move Move()
    Move combine(Move m);

    bool operator==(Move m);

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
