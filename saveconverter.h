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

        //if we're already using the latest version, then we don't need
        //to change anything
        if(fromVersion == Global::saveFormatVersion()){
            return data;
        }

        if(fromVersion == "0.0"){
            /* differences:
             * added settings object containing:
             * antialiasing, backgroundColour, lineColour, lineWidth
             */

            toVersion = "0.1";

            //create the settings json object with the default values
            //that were used in save format version 0.1
            QJsonObject settings;
            settings["antialiasing"] = false;
            settings["backgroundColour"] = "#ffffe4c4";
            settings["lineColour"] = "#ff000000";
            settings["lineWidth"] = 0;

            //add it to the save file
            data["settings"] = settings;
        }
        else if(fromVersion == "0.1"){
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
        }

        //update the version number
        data["version"] = toVersion;

        //keep converting the data forward one version at a time until
        //fromVersion == Global::saveFormatVersion() is true and the save is
        //fully updated
        return convert(data);
    }

};

#endif // SAVECONVERTER_H
