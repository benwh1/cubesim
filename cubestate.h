#ifndef CUBESTATE_H
#define CUBESTATE_H

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QSize>
#include <cmath>
#include "enums.h"
#include "move.h"

class CubeState : public QObject
{
    Q_OBJECT
public:
    explicit CubeState(QObject *parent = nullptr);

    int getSize();
    void setSize(int s);

    void move(Move m);

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

    //helper functions to make the public move function more readable
    void move(Axis axis, int layer, int amount);
    void moveCW(Axis axis, int layer);
    void moveHalf(Axis axis, int layer);
    void moveCCW(Axis axis, int layer);

    //rotates a single face, no inner slice pieces
    void rotateFace(Face f, int amount);
    void rotateFaceCW(Face f);
    void rotateFaceHalf(Face f);
    void rotateFaceCCW(Face f);

signals:
    void moveDone(Move move);
    void rotationDone(Move move);
    void cubeReset();
    void cubeScrambled();
    void cubeStateChanged(); //emitted when the state is changed in any other way (e.g. by calling copyFrom)
    void cubeSizeChanged();

};

#endif // CUBESTATE_H
