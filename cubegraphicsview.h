#ifndef CUBEGRAPHICSVIEW_H
#define CUBEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>
#include <QInputDialog>
#include "cubegraphicsobject.h"

class CubeGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CubeGraphicsView(QWidget *parent = nullptr);
    void initialize(Cube *cube);

protected:
    void keyReleaseEvent(QKeyEvent *event);

private:
    QGraphicsScene *scene;

    Cube *cube;
    CubeGraphicsObject *cubeGraphicsObject;

};

#endif // CUBEGRAPHICSVIEW_H
