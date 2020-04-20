#include "statistics.h"

Statistics::Statistics(Cube *cube, QObject *parent) :
    QObject(parent)
{
    this->cube = cube;

    reset();
}

qint64 Statistics::getTime(){
    if(active) return timer.elapsed() + additionalTime;
    else return additionalTime;
}

qint64 Statistics::getMoves(){
    return moves;
}

int Statistics::getTPS(){
    qint64 time = getTime();
    if(time == 0) return 2147483647;
    return qRound((1000000.*moves)/time);
}

int Statistics::getCubeSize(){
    return cube->getSize();
}

int Statistics::getNumberOfPieces(){
    int n = getCubeSize();
    auto c = [](int n){ return n*n*n; };

    if(n%2 == 0){
        return c(n)-c(n-2);
    }
    else{
        return c(n)-c(n-2)-6;
    }
}

qint64 Statistics::getPiecesPerSecond(){
    return 1000000/getTimePerPiece();
}

qint64 Statistics::getTimePerPiece(){
    int pieces = getNumberOfPieces();
    qint64 time = getTime();
    return time/pieces;
}

qint64 Statistics::getPiecesPerMove(){
    return 1000000/getMovesPerPiece();
}

qint64 Statistics::getMovesPerPiece(){
    int pieces = getNumberOfPieces();
    qint64 moves = getMoves();
    return 1000*moves/pieces;
}

void Statistics::setTime(qint64 time){
    additionalTime = time;

    if(timerRunning()){
        timer.restart();
    }

    emit statisticChanged();
}

void Statistics::setMoves(qint64 moves){
    this->moves = moves;

    emit statisticChanged();
}

void Statistics::startTimer(){
    timer.start();
    active = true;

    emit timerStarted();
}

void Statistics::stopTimer(){
    //store the total time so that if we start the timer again, it will appear
    //as though it was just unpaused instead of restarting from zero
    additionalTime += timer.elapsed();
    active = false;

    emit timerStopped();
}

void Statistics::reset(){
    additionalTime = 0;
    moves = 0;
    active = false;

    emit timerReset();
}

void Statistics::doMove(){
    moves++;

    emit statisticChanged();
}

bool Statistics::timerRunning(){
    return active;
}

QString Statistics::timeString(){
    qint64 time = getTime();

    if(time < 0) return "-";

    QString formattedTime;
    QTime t = QTime(0,0).addMSecs(time);
    int hours = time/3600000;

    if(time < 60000) formattedTime = t.toString("s.zzz");
    else if(hours > 0) formattedTime = QString::number(hours) + ":" + t.toString("mm:ss.zzz");
    else formattedTime = t.toString("m:ss.zzz");

    return formattedTime;
}

QString Statistics::movesString(){
    return QString::number(moves);
}

QString Statistics::tpsString(){
    int tps = getTPS();

    if(tps == 2147483647) return QChar(0x221E);
    else return QString::number((float)tps/1000, 'f', 3);
}

QJsonObject Statistics::toJSON(){
    QJsonObject data;

    data["time"] = getTime();
    data["moves"] = moves;

    return data;
}

void Statistics::fromJSON(QJsonObject data){
    additionalTime = data["time"].toInt();
    moves = data["moves"].toInt();
}
