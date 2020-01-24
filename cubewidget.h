#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QElapsedTimer>
#include <QWidget>
#include "cube.h"
#include "statistics.h"

namespace Ui {
class CubeWidget;
}

class CubeWidget : public QWidget
{
    Q_OBJECT

public:
    enum State{
        Neutral,
        Inspecting,
        Solving,
        Finished
    };

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
    Statistics *statistics;

    State state;

    bool swapCtrlShift;

private slots:
    void onMoveDrag(Cube::Axis axis, int layer, bool clockwise);
    void onMoveDone(Cube::Axis axis, int layer);
    void onCubeSolved();

};

#endif // CUBEWIDGET_H
