#ifndef CUBE_H
#define CUBE_H

#include <QObject>
#include "cubestate.h"
#include "move.h"
#include "settings.h"

class Cube : public QObject
{
    Q_OBJECT
public:
    explicit Cube(Settings *settings, QObject *parent = nullptr);

    int getSize();
    void setSize(int s);

    void move(Move m);
    bool isSolved();

    int sticker(Face f, int x, int y);
    int stickerOrientation(Face f, int x, int y);

    CubeState *getState();
    CubeState *getLastScramble();

    void setState(CubeState *state);

    void reset();
    void scramble();

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    CubeState *state;
    CubeState *lastScramble;

    Settings *settings;

signals:
    void moveDone(Move move);
    void rotationDone(Move move);

    void cubeReset();
    void cubeScrambled();
    void cubeStateChanged();
    void cubeSolved();
    void cubeSizeChanged();

};

#endif // CUBE_H
