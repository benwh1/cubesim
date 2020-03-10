#include "move.h"

#include "cube.h"

Cube::Cube(Settings *settings, QObject *parent) :
    QObject(parent)
{
    this->settings = settings;
    state = new CubeState(this);
    lastScramble = new CubeState(this);

    //propogate signals from state
    connect(state, SIGNAL(moveDone(Axis,int,int,int)), this, SIGNAL(moveDone(Axis,int,int,int)));
    connect(state, SIGNAL(rotationDone(Axis,int)), this, SIGNAL(rotationDone(Axis,int)));
    connect(state, SIGNAL(cubeReset()), this, SIGNAL(cubeReset()));
    connect(state, SIGNAL(cubeScrambled()), this, SIGNAL(cubeScrambled()));
    connect(state, SIGNAL(cubeStateChanged()), this, SIGNAL(cubeStateChanged()));
    connect(state, SIGNAL(cubeSizeChanged()), this, SIGNAL(cubeSizeChanged()));
}

int Cube::getSize(){
    return state->getSize();
}

void Cube::setSize(int s){
    state->setSize(s);
}

void Cube::move(Move m){
    state->move(m);
    if(isSolved()) emit cubeSolved();
}

void Cube::move(Axis axis, int layer, int amount){
    state->move(axis, layer, amount);
    if(isSolved()) emit cubeSolved();
}

void Cube::multisliceMove(Axis axis, int layer, int amount){
    state->multisliceMove(axis, layer, amount);
    if(isSolved()) emit cubeSolved();
}

void Cube::rotate(Axis axis, int amount){
    state->rotate(axis, amount);
}

bool Cube::isSolved(){
    return state->isSolved(settings->getSupercube());
}

int Cube::sticker(Face f, int x, int y){
    return state->sticker(f, x, y);
}

int Cube::stickerOrientation(Face f, int x, int y){
    return state->stickerOrientation(f, x, y);
}

CubeState *Cube::getState(){
    return state;
}

CubeState *Cube::getLastScramble(){
    return lastScramble;
}

void Cube::setState(CubeState *state){
    state->copyFrom(state);
}

void Cube::reset(){
    state->reset();
    lastScramble->reset();
}

void Cube::scramble(){
    reset();

    state->scramble();
    lastScramble->copyFrom(state);
}

QJsonObject Cube::toJSON(){
    QJsonObject data;

    data["state"] = state->toJSON();
    data["lastScramble"] = lastScramble->toJSON();

    return data;
}

void Cube::fromJSON(QJsonObject data){
    state->fromJSON(data["state"].toObject());
    lastScramble->fromJSON(data["lastScramble"].toObject());
}
