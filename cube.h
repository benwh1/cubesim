#ifndef CUBE_H
#define CUBE_H

#include <QObject>
#include "cubestate.h"
#include "settings.h"

class Cube : public QObject
{
    Q_OBJECT
public:
    explicit Cube(Settings *settings, QObject *parent = nullptr);

    int getSize();
    void setSize(int s);

    void move(Axis axis, int layer, int amount);
    void multisliceMove(Axis axis, int layer, int amount);
    void rotate(Axis axis, int amount);

    bool isSolved();

    int sticker(Face f, int x, int y);
    int stickerOrientation(Face f, int x, int y);

    CubeState *getState();
    CubeState *getLastScramble();

    void reset();
    void scramble();

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    CubeState *state;
    CubeState *lastScramble;

    Settings *settings;

signals:
    void moveDone(Axis axis, int layerStart, int layerEnd, int amount);
    void rotationDone(Axis axis, int amount);

    void cubeReset();
    void cubeScrambled();
    void cubeSolved();
    void cubeSizeChanged();

};

#endif // CUBE_H
