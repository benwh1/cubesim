#include "timedbool.h"

TimedBool::TimedBool(bool defaultValue, int duration, QObject *parent) :
    QObject(parent),
    value(defaultValue),
    defaultValue(defaultValue)
{
    timer.setSingleShot(true);
    timer.setInterval(duration);

    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void TimedBool::setDuration(int d){
    timer.setInterval(d);
}

TimedBool& TimedBool::operator=(const bool b){
    value = b;
    timer.start();
    return *this;
}

bool TimedBool::operator==(const bool b) const{
    return value == b;
}

bool TimedBool::operator!() const{
    return !value;
}

void TimedBool::onTimeout(){
    value = defaultValue;
}
