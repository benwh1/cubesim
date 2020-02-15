#include "move.h"

Move::Move(Axis axis, int layerStart, int layerEnd, int amount)
{
    this->axis = axis;
    this->layerStart = layerStart;
    this->layerEnd = layerEnd;
    this->amount = amount;
}

QString Move::toString(){
    QString face, axis;
    if(this->axis == Axis::X){
        face = "R";
        axis = "x";
    }
    else if(this->axis == Axis::Y){
        face = "U";
        axis = "y";
    }
    else{
        face = "F";
        axis = "z";
    }

    QString suffix;
    if(amount == 1) suffix = "";
    else if(amount == 2) suffix = "2";
    else if(amount == 3) suffix = "'";

    //cube rotation
    if(layerStart == 0 && layerEnd == -1){
        return axis + suffix;
    }

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

    return layers + face + suffix;
}
