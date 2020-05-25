#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

class Global
{
public:
    static QString version(){
        return "0.6.1";
    }

    static QString saveFormatVersion(){
        return "0.7";
    }

};

#endif // GLOBAL_H
