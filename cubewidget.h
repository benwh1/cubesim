#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QWidget>
#include "cube.h"

namespace Ui {
class CubeWidget;
}

class CubeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CubeWidget(QWidget *parent = nullptr);
    ~CubeWidget();

    void initialize(Cube *cube);

    bool multislice;

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::CubeWidget *ui;

    Cube *cube;

private slots:
    void onMoveDrag(Cube::Axis axis, int layer, bool clockwise);

};

#endif // CUBEWIDGET_H
