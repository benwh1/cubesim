#include "reconstruction.h"

Reconstruction::Reconstruction()
{

}

void Reconstruction::addMove(Axis axis, int layerStart, int layerEnd, int amount, qint64 time){
    Move m(axis, layerStart, layerEnd, amount);
    moves.append(QPair<Move, qint64>(m, time));
}

void Reconstruction::addRotation(Axis axis, int amount, qint64 time){
    //use -1 as the end layer to denote a rotation
    Move m(axis, 0, -1, amount);
    moves.append(QPair<Move, qint64>(m, time));
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

QJsonObject Reconstruction::toJSON(){
    QJsonObject data;

    QJsonArray m, t;
    for(int i=0; i<moves.size(); i++){
        m.append(moves[i].first.toJSON());
        t.append(moves[i].second);
    }

    data["moves"] = m;
    data["times"] = t;

    return data;
}

void Reconstruction::fromJSON(QJsonObject data){
    moves.clear();

    QJsonArray m = data["moves"].toArray();
    QJsonArray t = data["times"].toArray();

    Move move;
    qint64 time;
    for(int i=0; i<m.size(); i++){
        move.fromJSON(m[i].toObject());
        time = t[i].toInt();
        moves.append(QPair<Move, qint64>(move, time));
    }
}
