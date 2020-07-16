#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

class Global
{
public:
    static QString version(){
        return "1.2";
    }

    static QString saveFormatVersion(){
        return "1.1";
    }

};

#endif // GLOBAL_H
