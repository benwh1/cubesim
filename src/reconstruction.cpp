#include "reconstruction.h"

Reconstruction::Reconstruction(Cube *cube, Statistics *statistics, QObject *parent) :
    QObject(parent)
{
    this->cube = cube;
    this->statistics = statistics;

    active = false;

    //connect to cube signals
    connect(cube, SIGNAL(moveDone(Move)), this, SLOT(onMoveDone(Move)));
    connect(cube, SIGNAL(rotationDone(Move)), this, SLOT(onRotationDone(Move)));
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
    if(moves.length() == 0){
        return 0;
    }
    else{
        return moves.last().second;
    }
}

int Reconstruction::numMoves(){
    int n = 0;
    for(int i=0; i<moves.size(); i++){
        if(!moves[i].first.isRotation()){
            n++;
        }
    }
    return n;
}

int Reconstruction::numRotations(){
    return length() - numMoves();
}

void Reconstruction::start(){
    active = true;
}

void Reconstruction::finish(){
    active = false;
}

void Reconstruction::reset(){
    moves.clear();
    active = false;
}

void Reconstruction::simplify(){
    for(int i=0; i<length()-1; i++){
        //if length is 1, there is nothing left to do
        if(length() == 1){
            break;
        }

        //make sure i >= 0. it is possible that i<0 here because if the first
        //two moves are U U', then we will remove them and go backwards a move
        if(i<0) i=0;

        //current move and the next move
        Move m1, m2;
        m1 = moves[i].first;
        m2 = moves[i+1].first;

        //combine the moves if possible
        Move m3 = m1.combine(m2);

        //check that the moves cancel
        if(!(m3 == Move())){
            //check if they fully cancel (like U U', but not U U2)
            if(m3.getAmount() == 0){
                //remove m1 and m2 from the reconstruction
                moves.removeAt(i+1);
                moves.removeAt(i);
            }
            //moves cancel, but not completely
            else{
                //replace the two original moves with the combined move
                //by overwriting the first move and removing the second move
                moves[i].first = m3;
                moves.removeAt(i+1);
            }

            //go backwards one move so we don't miss any simplifications
            //e.g. we could have R U U' R', remove the U U', then we need to
            //go back to the R move to see that R R' simplifies.
            //we also run i++ before the next iteration of the loop, so we
            //need to subtract 2 from i here instead of 1
            i -= 2;
            continue;
        }
    }
}

Reconstruction *Reconstruction::simplified(){
    //Qt doesn't allow subclasses of QObject to be copied (because it's not
    //clear what should happen with signals and slots), so copy manually
    Reconstruction *r = new Reconstruction(cube, statistics, parent());
    for(int i=0; i<moves.size(); i++){
        r->addMove(moves[i].first, moves[i].second);
    }

    r->simplify();
    return r;
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
    data["active"] = active;

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

    active = data["active"].toBool();
}

void Reconstruction::onMoveDone(Move move){
    if(active){
        addMove(move, statistics->getTime());
    }
}

void Reconstruction::onRotationDone(Move move){
    if(active){
        addRotation(move, statistics->getTime());
    }
}
