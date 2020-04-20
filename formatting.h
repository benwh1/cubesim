#ifndef FORMATTING_H
#define FORMATTING_H

#include <QString>
#include <QTime>

class Formatting
{
public:
    static QString formatTime(qint64 time){
        if(time < 0) return "-" + Formatting::formatTime(-time);

        QTime t = QTime(0,0).addMSecs(time);
        int hours = time/3600000;

        if(time < 60000){
            return t.toString("s.zzz");
        }
        else if(hours > 0){
            return QString::number(hours) + ":" + t.toString("mm:ss.zzz");
        }
        else{
            return t.toString("m:ss.zzz");
        }
    }

    static QString formatMovecount(qint64 moves){
        return QString::number(moves);
    }

    static QString formatTPS(qint64 tps){
        if(tps == 2147483647){
            return QChar(0x221E);
        }
        else{
            return QString::number((float)tps/1000, 'f', 3);
        }
    }

};

#endif // FORMATTING_H
