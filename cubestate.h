#ifndef CUBESTATE_H
#define CUBESTATE_H

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QSize>
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

    //this should only be used internally - doesn't emit moveDone signal
    void move(Axis axis, int layer);

    //rotates a single face, no inner slice pieces
    void rotateFace(Face f);
    void rotateFace(Face f, int amount);

signals:
    void moveDone(Move move);
    void rotationDone(Move move);
    void cubeReset();
    void cubeScrambled();
    void cubeStateChanged(); //emitted when the state is changed in any other way (e.g. by calling copyFrom)
    void cubeSizeChanged();

};

#endif // CUBESTATE_H
