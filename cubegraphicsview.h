#ifndef CUBEGRAPHICSVIEW_H
#define CUBEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QInputDialog>
#include "cubegraphicsobject.h"

class CubeGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CubeGraphicsView(QWidget *parent = nullptr);
    void initialize(Cube *cube);

    void zoom(qreal factor);

    void setCubeProjection(QString matrix);
    void setCubeProjection(float *matrix);
    void resetCubeProjection();

    CubeGraphicsObject *getCubeGraphicsObject();

private:
    QGraphicsScene *scene;

    Cube *cube;
    CubeGraphicsObject *cubeGraphicsObject;

    qreal zoomFactor;

private slots:
    void onProjectionChanged();

signals:
    void moveDrag(Cube::Axis axis, int layer, bool clockwise);

};

#endif // CUBEGRAPHICSVIEW_H
