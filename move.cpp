#include "move.h"

Move::Move(Axis axis, int layerStart, int layerEnd, int amount)
{
    this->axis = axis;
    this->layerStart = layerStart;
    this->layerEnd = layerEnd;
    this->amount = amount;
}

QString Move::toString(){
    QString axis;
    if(this->axis == Axis::X) axis = "R";
    else if(this->axis == Axis::Y) axis = "U";
    else axis = "F";

    QString layers;
    if(layerStart == 0){
        if(layerEnd == 0) layers = "";
        else layers = QString::number(layerEnd+1);
    }
    else{
        if(layerStart == layerEnd){
            layers = QString::number(layerStart+1);
        }
        else{
            layers = QString::number(layerStart+1) + "-" + QString::number(layerEnd+1);
        }
    }

    QString suffix;
    if(amount == 1) suffix = "";
    else if(amount == 2) suffix = "2";
    else if(amount == 3) suffix = "'";

    return layers + axis + suffix;
}
