#ifndef SAVECONVERTER_H
#define SAVECONVERTER_H

#include <QJsonArray>
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
             * added guide lines to Settings
             */

            toVersion = "0.2";

            //get the multislice variable and remove it
            bool multislice = data["multislice"].toBool();
            data.remove("multislice");

            //add the new variables into settings
            QJsonObject settings = data["settings"].toObject();
            settings["multislice"] = multislice;
            settings["guideLinesCross"] = false;
            settings["guideLinesPlus"] = false;
            settings["guideLinesBox"] = false;
            settings["guideLineColour"] = "#ff000000";
            settings["guideLineWidth"] = 2;

            //add the new settings to the save file
            data["settings"] = settings;
        }
        else if(fromVersion == "0.2"){
            /* differences:
             * added colour scheme to Settings
             * added supercube to settings
             */

            toVersion = "0.3";

            //create the default colour scheme array
            QJsonArray colourArray;
            colourArray.append("#ffffffff"); //white
            colourArray.append("#ff00ff00"); //green
            colourArray.append("#ffff0000"); //red
            colourArray.append("#ff0000ff"); //blue
            colourArray.append("#ffffa500"); //orange
            colourArray.append("#ffffff00"); //yellow

            //add it into settings
            QJsonObject settings = data["settings"].toObject();
            settings["colours"] = colourArray;

            //add supercube to settings
            settings["supercube"] = false;

            //add the new settings to the save file
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
