#include "cubestate.h"

CubeState::CubeState(QObject *parent) :
    QObject(parent)
{
    setSize(3);
}

int CubeState::getSize(){
    return size;
}

void CubeState::setSize(int s){
    size = s;
    if(size < 2) size = 2;

    blockSignals(true);
    reset();
    blockSignals(false);

    emit cubeSizeChanged();
}

void CubeState::move(Move m){
    Axis axis = m.getAxis();
    int layerStart = m.getLayerStart();
    int layerEnd = m.getLayerEnd();
    int amount = m.getAmount();

    if(m.isRotation()){
        if(axis == Axis::X){
            rotateFace(Face::R, amount);
            rotateFace(Face::L, 4-amount);

            for(int i=0; i<amount; i++){
                rotateFace(Face::U, 2);
                rotateFace(Face::B, 2);

                QList<QList<int>> temp = stickers[Face::U];
                stickers[Face::U] = stickers[Face::F];
                stickers[Face::F] = stickers[Face::D];
                stickers[Face::D] = stickers[Face::B];
                stickers[Face::B] = temp;

                temp = orientations[Face::U];
                orientations[Face::U] = orientations[Face::F];
                orientations[Face::F] = orientations[Face::D];
                orientations[Face::D] = orientations[Face::B];
                orientations[Face::B] = temp;
            }
        }
        else if(axis == Axis::Y){
            rotateFace(Face::U, amount);
            rotateFace(Face::D, 4-amount);

            for(int i=0; i<amount; i++){
                QList<QList<int>> temp = stickers[Face::F];
                stickers[Face::F] = stickers[Face::R];
                stickers[Face::R] = stickers[Face::B];
                stickers[Face::B] = stickers[Face::L];
                stickers[Face::L] = temp;

                temp = orientations[Face::F];
                orientations[Face::F] = orientations[Face::R];
                orientations[Face::R] = orientations[Face::B];
                orientations[Face::B] = orientations[Face::L];
                orientations[Face::L] = temp;
            }
        }
        else if(axis == Axis::Z){
            rotateFace(Face::F, amount);
            rotateFace(Face::B, 4-amount);

            rotateFace(Face::U, amount);
            rotateFace(Face::L, amount);
            rotateFace(Face::D, amount);
            rotateFace(Face::R, amount);

            for(int i=0; i<amount; i++){
                QList<QList<int>> temp = stickers[Face::U];
                stickers[Face::U] = stickers[Face::L];
                stickers[Face::L] = stickers[Face::D];
                stickers[Face::D] = stickers[Face::R];
                stickers[Face::R] = temp;

                temp = orientations[Face::U];
                orientations[Face::U] = orientations[Face::L];
                orientations[Face::L] = orientations[Face::D];
                orientations[Face::D] = orientations[Face::R];
                orientations[Face::R] = temp;
            }
        }

        emit rotationDone(m);
    }
    else{
        for(int layer=layerStart; layer<=layerEnd; layer++){
            move(axis, layer, amount);
        }

        emit moveDone(m);
    }
}

void CubeState::moveCW(Axis axis, int layer){
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
            orientations[Face::U][size-1-layer][i] = (orientations[Face::L][size-1-i][size-1-layer]+1)%4;
            orientations[Face::L][size-1-i][size-1-layer] = (orientations[Face::D][layer][size-1-i]+1)%4;
            orientations[Face::D][layer][size-1-i] = (orientations[Face::R][i][layer]+1)%4;
            orientations[Face::R][i][layer] = (temp+1)%4;
        }

        if(layer == 0) rotateFace(Face::F, 1);
        else if(layer == size-1) rotateFace(Face::B, 3);
    }
}

void CubeState::moveHalf(Axis axis, int layer){
    if(axis == Axis::X){ //R-L moves
        //cycle U -> D -> U and B -> F -> B
        for(int i=0; i<size; i++){
            int temp = stickers[Face::U][i][size-1-layer];
            stickers[Face::U][i][size-1-layer] = stickers[Face::D][i][size-1-layer];
            stickers[Face::D][i][size-1-layer] = temp;

            temp = stickers[Face::B][size-1-i][layer];
            stickers[Face::B][size-1-i][layer] = stickers[Face::F][i][size-1-layer];
            stickers[Face::F][i][size-1-layer] = temp;

            temp = orientations[Face::U][i][size-1-layer];
            orientations[Face::U][i][size-1-layer] = orientations[Face::D][i][size-1-layer];
            orientations[Face::D][i][size-1-layer] = temp;

            temp = orientations[Face::B][size-1-i][layer];
            orientations[Face::B][size-1-i][layer] = (orientations[Face::F][i][size-1-layer]+2)%4;
            orientations[Face::F][i][size-1-layer] = (temp+2)%4;
        }

        if(layer == 0) rotateFace(Face::R, 2);
        else if(layer == size-1) rotateFace(Face::L, 2);
    }
    else if(axis == Axis::Y){ //U-D moves
        //cycle F -> B -> F and L -> R -> L
        for(int i=0; i<size; i++){
            int temp = stickers[Face::F][layer][i];
            stickers[Face::F][layer][i] = stickers[Face::B][layer][i];
            stickers[Face::B][layer][i] = temp;

            temp = stickers[Face::L][layer][i];
            stickers[Face::L][layer][i] = stickers[Face::R][layer][i];
            stickers[Face::R][layer][i] = temp;

            temp = orientations[Face::F][layer][i];
            orientations[Face::F][layer][i] = orientations[Face::B][layer][i];
            orientations[Face::B][layer][i] = temp;

            temp = orientations[Face::L][layer][i];
            orientations[Face::L][layer][i] = orientations[Face::R][layer][i];
            orientations[Face::R][layer][i] = temp;
        }

        if(layer == 0) rotateFace(Face::U, 2);
        else if(layer == size-1) rotateFace(Face::D, 2);
    }
    else if(axis == Axis::Z){ //F-B moves
        //cycle U -> D -> U and R -> L -> R
        for(int i=0; i<size; i++){
            int temp = stickers[Face::U][size-1-layer][i];
            stickers[Face::U][size-1-layer][i] = stickers[Face::D][layer][size-1-i];
            stickers[Face::D][layer][size-1-i] = temp;

            temp = stickers[Face::R][i][layer];
            stickers[Face::R][i][layer] = stickers[Face::L][size-1-i][size-1-layer];
            stickers[Face::L][size-1-i][size-1-layer] = temp;

            temp = orientations[Face::U][size-1-layer][i];
            orientations[Face::U][size-1-layer][i] = (orientations[Face::D][layer][size-1-i]+2)%4;
            orientations[Face::D][layer][size-1-i] = (temp+2)%4;

            temp = orientations[Face::R][i][layer];
            orientations[Face::R][i][layer] = (orientations[Face::L][size-1-i][size-1-layer]+2)%4;
            orientations[Face::L][size-1-i][size-1-layer] = (temp+2)%4;
        }

        if(layer == 0) rotateFace(Face::F, 2);
        else if(layer == size-1) rotateFace(Face::B, 2);
    }
}

void CubeState::moveCCW(Axis axis, int layer){
    if(axis == Axis::X){ //R-L moves
        //cycle U -> F -> D -> B -> U
        for(int i=0; i<size; i++){
            int temp = stickers[Face::U][i][size-1-layer];
            stickers[Face::U][i][size-1-layer] = stickers[Face::B][size-1-i][layer];
            stickers[Face::B][size-1-i][layer] = stickers[Face::D][i][size-1-layer];
            stickers[Face::D][i][size-1-layer] = stickers[Face::F][i][size-1-layer];
            stickers[Face::F][i][size-1-layer] = temp;

            temp = orientations[Face::U][i][size-1-layer];
            orientations[Face::U][i][size-1-layer] = (orientations[Face::B][size-1-i][layer]+2)%4;
            orientations[Face::B][size-1-i][layer] = (orientations[Face::D][i][size-1-layer]+2)%4;
            orientations[Face::D][i][size-1-layer] = orientations[Face::F][i][size-1-layer];
            orientations[Face::F][i][size-1-layer] = temp;
        }

        if(layer == 0) rotateFace(Face::R, 3);
        else if(layer == size-1) rotateFace(Face::L, 1);
    }
    else if(axis == Axis::Y){ //U-D moves
        //cycle F -> R -> B -> L -> F
        for(int i=0; i<size; i++){
            int temp = stickers[Face::F][layer][i];
            stickers[Face::F][layer][i] = stickers[Face::L][layer][i];
            stickers[Face::L][layer][i] = stickers[Face::B][layer][i];
            stickers[Face::B][layer][i] = stickers[Face::R][layer][i];
            stickers[Face::R][layer][i] = temp;

            temp = orientations[Face::F][layer][i];
            orientations[Face::F][layer][i] = orientations[Face::L][layer][i];
            orientations[Face::L][layer][i] = orientations[Face::B][layer][i];
            orientations[Face::B][layer][i] = orientations[Face::R][layer][i];
            orientations[Face::R][layer][i] = temp;
        }

        if(layer == 0) rotateFace(Face::U, 3);
        else if(layer == size-1) rotateFace(Face::D, 1);
    }
    else if(axis == Axis::Z){ //F-B moves
        //cycle U -> L -> D -> R -> U
        for(int i=0; i<size; i++){
            int temp = stickers[Face::U][size-1-layer][i];
            stickers[Face::U][size-1-layer][i] = stickers[Face::R][i][layer];
            stickers[Face::R][i][layer] = stickers[Face::D][layer][size-1-i];
            stickers[Face::D][layer][size-1-i] = stickers[Face::L][size-1-i][size-1-layer];
            stickers[Face::L][size-1-i][size-1-layer] = temp;

            temp = orientations[Face::U][size-1-layer][i];
            orientations[Face::U][size-1-layer][i] = (orientations[Face::R][i][layer]+3)%4;
            orientations[Face::R][i][layer] = (orientations[Face::D][layer][size-1-i]+3)%4;
            orientations[Face::D][layer][size-1-i] = (orientations[Face::L][size-1-i][size-1-layer]+3)%4;
            orientations[Face::L][size-1-i][size-1-layer] = (temp+3)%4;
        }

        if(layer == 0) rotateFace(Face::F, 3);
        else if(layer == size-1) rotateFace(Face::B, 1);
    }
}

void CubeState::rotateFace(Face f, int amount){
    if(amount == 1){
        rotateFaceCW(f);
    }
    else if(amount == 2){
        rotateFaceHalf(f);
    }
    else if(amount == 3){
        rotateFaceCCW(f);
    }
}

void CubeState::rotateFaceCW(Face f){
    QList<QList<int>> &face = stickers[f];
    QList<QList<int>> &ori = orientations[f];

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

            temp = ori[y][x];
            ori[y][x] = ori[size-1-x][y];
            ori[size-1-x][y] = ori[size-1-y][size-1-x];
            ori[size-1-y][size-1-x] = ori[x][size-1-y];
            ori[x][size-1-y] = temp;
        }
    }

    for(int y=0; y<size; y++){
        for(int x=0; x<size; x++){
            ori[y][x] = (ori[y][x]+1)%4;
        }
    }
}

void CubeState::rotateFaceHalf(Face f){
    QList<QList<int>> &face = stickers[f];
    QList<QList<int>> &ori = orientations[f];

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
            face[y][x] = face[size-1-y][size-1-x];
            face[size-1-y][size-1-x] = temp;

            temp = face[size-1-x][y];
            face[size-1-x][y] = face[x][size-1-y];
            face[x][size-1-y] = temp;

            temp = ori[y][x];
            ori[y][x] = ori[size-1-y][size-1-x];
            ori[size-1-y][size-1-x] = temp;

            temp = ori[size-1-x][y];
            ori[size-1-x][y] = ori[x][size-1-y];
            ori[x][size-1-y] = temp;
        }
    }

    for(int y=0; y<size; y++){
        for(int x=0; x<size; x++){
            ori[y][x] = (ori[y][x]+2)%4;
        }
    }
}

void CubeState::rotateFaceCCW(Face f){
    QList<QList<int>> &face = stickers[f];
    QList<QList<int>> &ori = orientations[f];

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
            face[y][x] = face[x][size-1-y];
            face[x][size-1-y] = face[size-1-y][size-1-x];
            face[size-1-y][size-1-x] = face[size-1-x][y];
            face[size-1-x][y] = temp;

            temp = ori[y][x];
            ori[y][x] = ori[x][size-1-y];
            ori[x][size-1-y] = ori[size-1-y][size-1-x];
            ori[size-1-y][size-1-x] = ori[size-1-x][y];
            ori[size-1-x][y] = temp;
        }
    }

    for(int y=0; y<size; y++){
        for(int x=0; x<size; x++){
            ori[y][x] = (ori[y][x]+3)%4;
        }
    }
}

bool CubeState::isSolved(bool super){
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
    if(super){
        for(int face=0; face<6; face++){
            int faceOrientation = orientations[face][0][0];
            for(int y=0; y<size; y++){
                for(int x=0; x<size; x++){
                    if(orientations[face][y][x] != faceOrientation){
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

QList<QList<QList<int> > > CubeState::getStickers(){
    return stickers;
}

QList<QList<QList<int> > > CubeState::getOrientations(){
    return orientations;
}

int CubeState::sticker(Face f, int x, int y){
    return stickers[f][y][x];
}

int CubeState::stickerOrientation(Face f, int x, int y){
    return orientations[f][y][x];
}

void CubeState::copyFrom(CubeState *c){
    setSize(c->getSize());
    stickers = c->getStickers();
    orientations = c->getOrientations();

    emit cubeStateChanged();
}

void CubeState::scramble(){
    int moves = 1000 + 9*size*size/log(size)+ qrand()%size;

    for(int i=0; i<moves; i++){
        move((Axis)(qrand()%3), qrand()%size, 1+qrand()%3);
    }

    emit cubeScrambled();
}

QJsonObject CubeState::toJSON(){
    QJsonObject data;

    data["size"] = size;

    QByteArray stickerArr;
    QByteArray orientationArr;

    stickerArr.resize(6*size*size);
    orientationArr.resize(6*size*size);

    for(int face=0; face<6; face++){
        for(int y=0; y<size; y++){
            for(int x=0; x<size; x++){
                int index = (face*size+y)*size+x;
                stickerArr[index] = stickers[face][y][x];
                orientationArr[index] = orientations[face][y][x];
            }
        }
    }

    data["stickers"] = QString(qCompress(stickerArr).toBase64());
    data["orientations"] = QString(qCompress(orientationArr).toBase64());

    return data;
}

void CubeState::fromJSON(QJsonObject data){
    setSize(data["size"].toInt());

    QByteArray stickerArr = qUncompress(QByteArray::fromBase64(data["stickers"].toString().toUtf8()));
    QByteArray orientationArr = qUncompress(QByteArray::fromBase64(data["orientations"].toString().toUtf8()));
    for(int face=0; face<6; face++){
        for(int y=0; y<size; y++){
            for(int x=0; x<size; x++){
                int index = (face*size+y)*size+x;

                stickers[face][y][x] = stickerArr[index];
                orientations[face][y][x] = orientationArr[index];
            }
        }
    }
}

void CubeState::move(Axis axis, int layer, int amount){
    if(amount == 1){
        moveCW(axis, layer);
    }
    else if(amount == 2){
        moveHalf(axis, layer);
    }
    else if(amount == 3){
        moveCCW(axis, layer);
    }
}

void CubeState::reset(){
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
