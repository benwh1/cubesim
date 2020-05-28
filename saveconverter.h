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
        else if(fromVersion == "0.5"){
            /* differences:
             * active parameter in reconstruction and statistics
             */

            toVersion = "0.6";

            //if state == 2 (Solving) then active == true, otherwise false
            //currently, we do not allow saving in state 1 (Inspecting), so
            //we only need to check if state == 2

            //get the state
            int state = data["state"].toInt();

            //get the old reconstruction and statistics from the save
            QJsonObject reconstruction = data["reconstruction"].toObject();
            QJsonObject statistics = data["statistics"].toObject();

            //add the active parameters
            reconstruction["active"] = (state == 2);
            statistics["active"] = (state == 2);

            //add the new reconstruction and statistics to the save
            data["reconstruction"] = reconstruction;
            data["statistics"] = statistics;
        }
        else if(fromVersion == "0.6"){
            /* differences:
             * none
             * bug fix:
             * if we do one solve, and then another, the rotations during
             * inspection of the second solve will be given the time of the
             * first solve, because the timer wasn't reset before inspection.
             * the first move after inspection is also given the time of the
             * previous solve.
             *
             * so we need to take the rotations at the start of the solve
             * and the first move, and set all of the times to 0
             */

            toVersion = "0.7";

            //get the lists of moves and times
            QJsonObject reconstruction = data["reconstruction"].toObject();
            QJsonArray moves = reconstruction["moves"].toArray();
            QJsonArray times = reconstruction["times"].toArray();

            for(int i=0; i<moves.size(); i++){
                QJsonObject move = moves[i].toObject();

                //in this version, the move is a rotation iff layerStart == 0
                //and layerEnd == -1
                if(move["layerStart"].toInt() == 0 &&
                   move["layerEnd"].toInt() == -1){
                    //if rotation, set the time to 0
                    times[i] = 0;
                }
                else{
                    //if not rotation, then we have finished inspection.
                    //fix the time of the first move, and then break
                    times[i] = 0;
                    break;
                }
            }

            //update the save file data
            reconstruction["moves"] = moves;
            reconstruction["times"] = times;
            data["reconstruction"] = reconstruction;
        }
        else if(fromVersion == "0.7"){
            /* differences:
             * added Controls
             * removed swapCtrlShift
             */

            toVersion = "0.8";

            //build what would have been the default Controls object for
            //save version 0.7
            QJsonObject controls;
            controls["scrambleShortcut"] = "Space";
            controls["resetShortcut"] = "Esc";
            controls["increaseSizeShortcut"] = "=";
            controls["decreaseSizeShortcut"] = "-";
            controls["changeSizeShortcut"] = "+";

            QJsonArray arr;
            for(int i=0; i<10; i++){
                arr.append(QString::number(i));
            }
            controls["loadProjectionShortcuts"] = arr;
            controls["resetProjectionShortcut"] = "D";
            controls["changeProjectionShortcut"] = "P";

            controls["resetZoomShortcut"] = "Home";
            controls["zoomInShortcut"] = "PgUp";
            controls["zoomInSmallShortcut"] = "Ctrl+PgUp";
            controls["zoomInLargeShortcut"] = "";
            controls["zoomOutShortcut"] = "PgDown";
            controls["zoomOutSmallShortcut"] = "Ctrl+PgDown";
            controls["zoomOutLargeShortcut"] = "";

            controls["saveShortcut"] = "Ctrl+S";
            controls["loadShortcut"] = "Ctrl+O";

            controls["toggleStatsShortcut"] = "V";

            controls["toggleMultisliceShortcut"] = "CapsLock";

            controls["screenshotShortcut"] = "Ctrl+P";

            controls["settingsWindowShortcut"] = "W";
            controls["replayRecorderWindowShortcut"] = "R";
            controls["reconstructionWindowShortcut"] = "L";

            controls["leftClickAction"] = 1;
            controls["ctrlClickAction"] = 3;
            controls["shiftClickAction"] = 2;
            controls["rightClickAction"] = 2;
            controls["middleClickAction"] = 3;

            //if swapCtrlShift was true, swap ctrlClick and shiftClick
            bool swapCtrlShift = data["swapCtrlShift"].toBool();
            if(swapCtrlShift){
                controls["ctrlClickAction"] = 2;
                controls["shiftClickAction"] = 3;
            }

            //add the controls to the settings
            QJsonObject settings = data["settings"].toObject();
            settings["controls"] = controls;
            data["settings"] = settings;

            //remove swapCtrlShift from the save file
            data.remove("swapCtrlShift");
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
