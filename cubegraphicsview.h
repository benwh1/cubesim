#ifndef CUBEGRAPHICSVIEW_H
#define CUBEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>
#include "cubegraphicsobject.h"

class CubeGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CubeGraphicsView(QWidget *parent = nullptr);
    void initialize(Cube *cube);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QGraphicsScene *scene;

    CubeGraphicsObject *cubeGraphicsObject;

};

#endif // CUBEGRAPHICSVIEW_H
