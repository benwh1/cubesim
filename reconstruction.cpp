#include "reconstruction.h"

Reconstruction::Reconstruction()
{

}

void Reconstruction::addMove(Axis axis, int layerStart, int layerEnd, int amount, qint64 time){
    Move m(axis, layerStart, layerEnd, amount);

    quint64 t;
    if(moves.length() == 0) t = 0;
    else t = time - moves.last().second;

    moves.append(QPair<Move, qint64>(m, t));
}

void Reconstruction::addRotation(Axis axis, int amount, qint64 time){
    //use -1 as the end layer to denote a rotation
    Move m(axis, 0, -1, amount);

    quint64 t;
    if(moves.length() == 0) t = 0;
    else t = time - moves.last().second;

    moves.append(QPair<Move, qint64>(m, t));
}

void Reconstruction::reset(){
    moves.clear();
}

QString Reconstruction::toString(){
    QString str = "";
    for(int i=0; i<moves.size(); i++){
        str += moves[i].first.toString() + " ";
    }
    str.chop(1);
    return str;
}
