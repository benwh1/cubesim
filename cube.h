#ifndef CUBE_H
#define CUBE_H

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QSize>
#include "enums.h"

class Cube : public QObject
{
    Q_OBJECT
public:
    explicit Cube(QObject *parent = nullptr);

    int getSize();
    void setSize(int s);

    void move(Axis axis, int layer, int amount);
    void multisliceMove(Axis axis, int layer, int amount);
    void rotate(Axis axis, int amount);

    bool isSupercube();

    bool isSolved();

    int sticker(Face f, int x, int y);
    int stickerOrientation(Face f, int x, int y);

    void reset();
    void scramble();

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    QList<QList<QList<int>>> stickers;
    int size;

    bool supercube;

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

    //only emitted when the cube is solved by a move, not e.g. by a reset
    void cubeSolved();

    void cubeSizeChanged();

};

#endif // CUBE_H
