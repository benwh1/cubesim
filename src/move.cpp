#include "move.h"

Move::Move()
{
    axis = (Axis)-1;
    layerStart = -1;
    layerEnd = -1;
    amount = -1;
}

Move::Move(Axis axis, int layerStart, int layerEnd, int amount)
{
    this->axis = axis;
    this->layerStart = layerStart;
    this->layerEnd = layerEnd;
    this->amount = amount;
}

bool Move::isRotation(){
    return layerStart == 0 && layerEnd == -1;
}

Axis Move::getAxis(){
    return axis;
}

int Move::getLayerStart(){
    return layerStart;
}

int Move::getLayerEnd(){
    return layerEnd;
}

int Move::getAmount(){
    return amount;
}

Move Move::inverse(){
    return Move(axis, layerStart, layerEnd, (4-amount)%4);
}

bool Move::cancelsWith(Move m){
    return axis == m.getAxis() &&
            layerStart == m.getLayerStart() &&
            layerEnd == m.getLayerEnd();
}

Move Move::combine(Move m){
    if(cancelsWith(m)){
        return Move(axis, layerStart, layerEnd, (amount + m.getAmount())%4);
    }
    else{
        return Move();
    }
}

bool Move::operator==(Move m){
    return m.getAxis() == axis &&
            m.getLayerStart() == layerStart &&
            m.getLayerEnd() == layerEnd &&
            m.getAmount() == amount;
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
        if(layerEnd == 0){
            layers = "";
        }
        else{
            layers = QString::number(layerEnd+1);
            face = face.toLower();
        }
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

QJsonObject Move::toJSON(){
    QJsonObject data;

    data["axis"] = axis;
    data["layerStart"] = layerStart;
    data["layerEnd"] = layerEnd;
    data["amount"] = amount;

    return data;
}

void Move::fromJSON(QJsonObject data){
    axis = (Axis)(data["axis"].toInt());
    layerStart = data["layerStart"].toInt();
    layerEnd = data["layerEnd"].toInt();
    amount = data["amount"].toInt();
}
