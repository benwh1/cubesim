#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

class Global
{
public:
    static QString version(){
        return "0.0";
    }

    static QString saveFormatVersion(){
        return "0.1";
    }

};

#endif // GLOBAL_H
