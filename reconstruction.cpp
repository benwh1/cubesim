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

QString Reconstruction::toString(){
    QString str = "";
    for(int i=0; i<moves.size(); i++){
        str += moves[i].first.toString() + " ";
    }
    str.chop(1);
    return str;
}
