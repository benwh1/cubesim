#ifndef GLOBAL_H
#define GLOBAL_H

#include <QVersionNumber>

class Global
{
public:
    static QVersionNumber version(){
        return QVersionNumber(1, 3);
    }

    static QVersionNumber saveFormatVersion(){
        return QVersionNumber(1, 3);
    }

};

#endif // GLOBAL_H
