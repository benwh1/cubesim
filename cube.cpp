#include "cube.h"

Cube::Cube(QObject *parent) :
    QObject(parent)
{
    supercube = false;

    setSize(3);
}

int Cube::getSize(){
    return size;
}

void Cube::setSize(int s){
    size = s;
    if(size < 2) size = 2;

    blockSignals(true);
    reset();
    blockSignals(false);

    emit cubeSizeChanged();
}

void Cube::move(Axis axis, int layer){
    if(axis == Axis::X){ //R-L moves
        //cycle U -> B -> D -> F -> U slice
        for(int i=0; i<size; i++){
            int temp = stickers[Face::U][i][size-1-layer];
            stickers[Face::U][i][size-1-layer] = stickers[Face::F][i][size-1-layer];
            stickers[Face::F][i][size-1-layer] = stickers[Face::D][i][size-1-layer];
            stickers[Face::D][i][size-1-layer] = stickers[Face::B][size-1-i][layer];
            stickers[Face::B][size-1-i][layer] = temp;

            temp = orientations[Face::U][i][size-1-layer];
            orientations[Face::U][i][size-1-layer] = orientations[Face::F][i][size-1-layer];
            orientations[Face::F][i][size-1-layer] = orientations[Face::D][i][size-1-layer];
            orientations[Face::D][i][size-1-layer] = (orientations[Face::B][size-1-i][layer]+2)%4;
            orientations[Face::B][size-1-i][layer] = (temp+2)%4;
        }

        if(layer == 0) rotateFace(Face::R, 1);
        else if(layer == size-1) rotateFace(Face::L, 3);
    }
    else if(axis == Axis::Y){ //U-D moves
        //cycle F -> L -> B -> R -> F slice
        for(int i=0; i<size; i++){
            int temp = stickers[Face::F][layer][i];
            stickers[Face::F][layer][i] = stickers[Face::R][layer][i];
            stickers[Face::R][layer][i] = stickers[Face::B][layer][i];
            stickers[Face::B][layer][i] = stickers[Face::L][layer][i];
            stickers[Face::L][layer][i] = temp;

            temp = orientations[Face::F][layer][i];
            orientations[Face::F][layer][i] = orientations[Face::R][layer][i];
            orientations[Face::R][layer][i] = orientations[Face::B][layer][i];
            orientations[Face::B][layer][i] = orientations[Face::L][layer][i];
            orientations[Face::L][layer][i] = temp;
        }

        if(layer == 0) rotateFace(Face::U, 1);
        else if(layer == size-1) rotateFace(Face::D, 3);
    }
    else if(axis == Axis::Z){ //F-B moves
        //cycle U -> R -> D -> L -> U
        for(int i=0; i<size; i++){
            int temp = stickers[Face::U][size-1-layer][i];
            stickers[Face::U][size-1-layer][i] = stickers[Face::L][size-1-i][size-1-layer];
            stickers[Face::L][size-1-i][size-1-layer] = stickers[Face::D][layer][size-1-i];
            stickers[Face::D][layer][size-1-i] = stickers[Face::R][i][layer];
            stickers[Face::R][i][layer] = temp;

            temp = orientations[Face::U][size-1-layer][i];
            orientations[Face::U][size-1-layer][i] = (1+orientations[Face::L][size-1-i][size-1-layer])%4;
            orientations[Face::L][size-1-i][size-1-layer] = (1+orientations[Face::D][layer][size-1-i])%4;
            orientations[Face::D][layer][size-1-i] = (1+orientations[Face::R][i][layer])%4;
            orientations[Face::R][i][layer] = (1+temp)%4;
        }

        if(layer == 0) rotateFace(Face::F, 1);
        else if(layer == size-1) rotateFace(Face::B, 3);
    }
}

void Cube::rotateFace(Face f){
    QList<QList<int>> &face = stickers[f];

    int xmax, ymax;
    if(size%2 == 0){
        xmax = size/2;
        ymax = size/2;
    }
    else{
        xmax = size/2;
        ymax = size/2 + 1;
    }

    for(int y=0; y<ymax; y++){
        for(int x=0; x<xmax; x++){
            int temp = face[y][x];
            face[y][x] = face[size-1-x][y];
            face[size-1-x][y] = face[size-1-y][size-1-x];
            face[size-1-y][size-1-x] = face[x][size-1-y];
            face[x][size-1-y] = temp;

            temp = orientations[f][y][x];
            orientations[f][y][x] = orientations[f][size-1-x][y];
            orientations[f][size-1-x][y] = orientations[f][size-1-y][size-1-x];
            orientations[f][size-1-y][size-1-x] = orientations[f][x][size-1-y];
            orientations[f][x][size-1-y] = temp;
        }
    }

    for(int y=0; y<size; y++){
        for(int x=0; x<size; x++){
            orientations[f][y][x] = (orientations[f][y][x]+1)%4;
        }
    }
}

void Cube::rotateFace(Face f, int amount){
    for(int i=0; i<amount; i++){
        rotateFace(f);
    }
}

void Cube::move(Axis axis, int layer, int amount){
    for(int i=0; i<amount; i++){
        move(axis, layer);
    }

    emit moveDone(axis, layer, layer, amount);

    if(isSolved()){
        emit cubeSolved();
    }
}

void Cube::multisliceMove(Cube::Axis axis, int layer, int amount){
    if(layer < size/2){
        blockSignals(true);
        for(int i=0; i<=layer; i++){
            move(axis, i, amount);
        }
        blockSignals(false);

        emit moveDone(axis, 0, layer, amount);
    }
    else if(size%2 == 1 && layer == (size-1)/2){
        blockSignals(true);
        move(axis, layer, amount);
        blockSignals(false);

        emit moveDone(axis, layer, layer, amount);
    }
    else{
        blockSignals(true);
        for(int i=layer; i<size; i++){
            move(axis, i, amount);
        }
        blockSignals(false);

        emit moveDone(axis, layer, size-1, amount);
    }

    if(isSolved()){
        emit cubeSolved();
    }
}

void Cube::rotate(Cube::Axis axis, int amount){
    for(int i=0; i<size; i++){
        for(int n=0; n<amount; n++){
            move(axis, i);
        }
    }

    emit rotationDone(axis, amount);
}

bool Cube::isSupercube(){
    return supercube;
}

bool Cube::isSolved(){
    for(int face=0; face<6; face++){
        int faceColour = stickers[face][0][0];
        for(int y=0; y<size; y++){
            for(int x=0; x<size; x++){
                if(stickers[face][y][x] != faceColour){
                    return false;
                }
            }
        }
    }

    //check if all stickers are oriented correctly if this is a supercube
    if(supercube){
        for(int face=0; face<6; face++){
            for(int y=0; y<size; y++){
                for(int x=0; x<size; x++){
                    if(orientations[face][y][x] != 0){
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

int Cube::sticker(Face f, int x, int y){
    return stickers[f][y][x];
}

int Cube::stickerOrientation(Face f, int x, int y){
    return orientations[f][y][x];
}

void Cube::scramble(){
    int moves = 1000 + 10*size*size + qrand()%size;

    for(int i=0; i<moves; i++){
        move((Cube::Axis)(qrand()%3), rand()%size);
    }

    emit cubeScrambled();
}

QJsonObject Cube::toJSON(){
    QJsonObject data;

    QJsonArray stickerArray;
    for(int face=0; face<6; face++){
        for(int y=0; y<size; y++){
            for(int x=0; x<size; x++){
                stickerArray.append(stickers[face][y][x]);
            }
        }
    }

    data["size"] = size;
    data["stickers"] = stickerArray;

    return data;
}

void Cube::fromJSON(QJsonObject data){
    QJsonArray stickerArray;

    setSize(data["size"].toInt());
    stickerArray = data["stickers"].toArray();

    for(int face=0; face<6; face++){
        for(int y=0; y<size; y++){
            for(int x=0; x<size; x++){
                int index = (face*size+y)*size+x;
                int sticker = stickerArray[index].toInt(-1);
                if(sticker == -1) return;
                stickers[face][y][x] = sticker;
            }
        }
    }
}

void Cube::reset(){
    stickers.clear();
    orientations.clear();

    for(int face=0; face<6; face++){
        stickers.append(QList<QList<int>>());
        orientations.append(QList<QList<int>>());
        for(int y=0; y<size; y++){
            stickers[face].append(QList<int>());
            orientations[face].append(QList<int>());
            for(int x=0; x<size; x++){
                stickers[face][y].append(face);
                orientations[face][y].append(0);
            }
        }
    }

    emit cubeReset();
}
