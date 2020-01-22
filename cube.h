#ifndef CUBE_H
#define CUBE_H

#include <QDebug>
#include <QObject>
#include <QSize>

class Cube : public QObject
{
    Q_OBJECT
public:
    enum Face{
        U, F, R, B, L, D
    };

    enum Axis{
        X, Y, Z
    };

    explicit Cube(QObject *parent = nullptr);
    int getSize();
    void setSize(int s);
    void move(Axis axis, int layer, int amount);
    int sticker(Face f, int x, int y);

private:
    QList<QList<QList<int>>> stickers;
    int size;

    void reset();

    //this should only be used internally - doesn't emit moveDone signal
    void move(Axis axis, int layer);

    //rotates a single face, no inner slice pieces
    void rotateFace(Face f);
    void rotateFace(Face f, int amount);

signals:
    void moveDone(Cube::Axis axis, int layer);
    void moveDone(Cube::Axis axis, int layer, int amount);

};

#endif // CUBE_H
