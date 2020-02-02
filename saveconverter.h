#ifndef SAVECONVERTER_H
#define SAVECONVERTER_H

#include <QJsonObject>
#include "global.h"

class SaveConverter
{
public:
    static QJsonObject convert(QJsonObject data){
        QString fromVersion = data["version"].toString();
        QString toVersion;

        if(fromVersion == "0.1"){
            /* differences:
             * multislice moved into the Settings object
             */

            toVersion = "0.2";

            //get the multislice variable and remove it
            bool multislice = data["multislice"].toBool();
            data.remove("multislice");

            //add the multislice variable into settings
            QJsonObject settings = data["settings"].toObject();
            settings["multislice"] = multislice;
            data["settings"] = settings;

            //update the version number
            data["version"] = toVersion;
        }
        else if(fromVersion == Global::saveFormatVersion()){
            return data;
        }

        return convert(data);
    }

};

#endif // SAVECONVERTER_H
