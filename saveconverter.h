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
             * added supercube settings
             * added supercube orientations to Cube
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

            //add supercube settings
            settings["supercube"] = false;
            settings["supercubeStickers"] = "Arrows";
            settings["pochmannBarThickness"] = 0.2;

            //add the new settings to the save file
            data["settings"] = settings;

            //get the cube object and read the size
            QJsonObject cube = data["cube"].toObject();
            int size = cube["size"].toInt();

            //build the array
            QJsonArray orientationsArray;
            for(int face=0; face<6; face++){
                for(int y=0; y<size; y++){
                    for(int x=0; x<size; x++){
                        //default orientations are all 0
                        orientationsArray.append(0);
                    }
                }
            }

            //add it to the cube object
            cube["orientations"] = orientationsArray;

            //add the new cube to the save file
            data["cube"] = cube;
        }
        else if(fromVersion == "0.3"){
            /* differences:
             * added pochmann cage setting
             */

            toVersion = "0.3.1";

            //add pochmannCage to settings and update the save data
            QJsonObject settings = data["settings"].toObject();
            settings["pochmannCage"] = true;
            data["settings"] = settings;
        }
        else if(fromVersion == "0.3.1"){
            /* differences:
             * added reconstructions
             * added state
             * added statisticsWidget
             * cube contains state and lastScramble
             */

            toVersion = "0.4";

            QJsonObject reconstruction;

            //these store the list of moves and time between moves
            //but we have no data saved, so just make them empty lists
            QJsonArray emptyArray;
            reconstruction["moves"] = emptyArray;
            reconstruction["times"] = emptyArray;

            //add it to the save file
            data["reconstruction"] = reconstruction;

            //previously, all saves were in the Solving (== 2) state
            data["state"] = 2;

            //only thing stored in statisticsWidget is the style sheet
            //and since we are in the Solving state, the style sheet is blank
            QJsonObject statisticsWidget;
            statisticsWidget["timeStyleSheet"] = "";
            data["statisticsWidget"] = statisticsWidget;

            //make a new cube object and put the old cube in the new location
            QJsonObject oldCube = data["cube"].toObject();
            data.remove("cube");

            QJsonObject cube;
            cube["state"] = oldCube;

            //create another cube state, just using solved as the scramble
            int size = oldCube["size"].toInt();

            //make an array of 6*size^2 stickers
            QJsonArray scrambleStickers;
            for(int face=0; face<6; face++){
                for(int i=0; i<size*size; i++){
                    scrambleStickers.append(face);
                }
            }

            //make an array of 6*size^2 zeros for the supercube orientations
            QJsonArray scrambleOrientations;
            for(int i=0; i<6*size*size; i++){
                scrambleOrientations.append(0);
            }

            QJsonObject lastScrambleObject;
            lastScrambleObject["size"] = size;
            lastScrambleObject["stickers"] = scrambleStickers;
            lastScrambleObject["orientations"] = scrambleOrientations;

            //add the lastScramble to the new cube object
            cube["lastScramble"] = lastScrambleObject;

            //finally, add the new cube to the save file
            data["cube"] = cube;
        }
        else if(fromVersion == "0.4"){
            /* differences:
             * cubeGraphicsObject is stored in graphicsView
             */

            toVersion = "0.5";

            //move cubeGraphicsObject into graphicsView
            QJsonObject graphicsView;
            graphicsView["cubeGraphicsObject"] = data["cubeGraphicsObject"];
            data.remove("cubeGraphicsObject");
            data["graphicsView"] = graphicsView;
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
