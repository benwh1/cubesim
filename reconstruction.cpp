#include "reconstruction.h"

Reconstruction::Reconstruction(Cube *cube, Statistics *statistics, QObject *parent) :
    QObject(parent)
{
    this->cube = cube;
    this->statistics = statistics;
}

void Reconstruction::addMove(Move move, qint64 time){
    moves.append(QPair<Move, qint64>(move, time));
}

void Reconstruction::addRotation(Move move, qint64 time){
    moves.append(QPair<Move, qint64>(move, time));
}

Move Reconstruction::getMove(int n){
    assert(0 <= n && n < moves.size());
    return moves[n].first;
}

QPair<Move, qint64> Reconstruction::at(int n){
    assert(0 <= n && n < moves.size());
    return moves[n];
}

int Reconstruction::length(){
    return moves.size();
}

qint64 Reconstruction::totalTime(){
    if(moves.length() == 0) return 0;
    return moves.last().second;
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
