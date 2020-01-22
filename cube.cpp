#include "cube.h"

Cube::Cube(QObject *parent) :
    QObject(parent)
{
    setSize(20);
}

int Cube::getSize(){
    return size;
}

void Cube::setSize(int s){
    size = s;
    reset();
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

    emit moveDone(axis, layer);
    emit moveDone(axis, layer, amount);
}

int Cube::sticker(Face f, int x, int y){
    return stickers[f][y][x];
}

void Cube::reset(){
    stickers.clear();

    for(int face=0; face<6; face++){
        stickers.append(QList<QList<int>>());
        for(int y=0; y<size; y++){
            stickers[face].append(QList<int>());
            for(int x=0; x<size; x++){
                stickers[face][y].append(face);
            }
        }
    }
}
