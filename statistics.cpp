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
