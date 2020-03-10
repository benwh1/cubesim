#ifndef CUBESTATE_H
#define CUBESTATE_H

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QSize>
#include "enums.h"

class Move;

class CubeState : public QObject
{
    Q_OBJECT
public:
    explicit CubeState(QObject *parent = nullptr);

    int getSize();
    void setSize(int s);

    void move(Move m);
    void move(Axis axis, int layer, int amount);
    void move(Axis axis, int layerStart, int layerEnd, int amount);
    void multisliceMove(Axis axis, int layer, int amount);
    void rotate(Axis axis, int amount);

    bool isSolved(bool super = false);

    QList<QList<QList<int>>> getStickers();
    QList<QList<QList<int>>> getOrientations();

    int sticker(Face f, int x, int y);
    int stickerOrientation(Face f, int x, int y);

    //set the current objects size, stickers, orientations to match c
    void copyFrom(CubeState *c);

    void reset();
    void scramble();

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    int size;
    QList<QList<QList<int>>> stickers;

    //sticker orientations for supercubes
    //oriented correctly = 0
    //rotated clockwise once (so needs rotating ccw once to be solved) = 1
    //etc.
    QList<QList<QList<int>>> orientations;

    //this should only be used internally - doesn't emit moveDone signal
    void move(Axis axis, int layer);

    //rotates a single face, no inner slice pieces
    void rotateFace(Face f);
    void rotateFace(Face f, int amount);

signals:
    void moveDone(Axis axis, int layerStart, int layerEnd, int amount);
    void rotationDone(Axis axis, int amount);
    void cubeReset();
    void cubeScrambled();
    void cubeSizeChanged();

};

#endif // CUBESTATE_H
