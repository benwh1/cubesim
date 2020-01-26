#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QElapsedTimer>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
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

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::CubeWidget *ui;

    Cube *cube;
    Statistics *statistics;

    State state;

    bool multislice;
    bool swapCtrlShift;

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

    void save();
    void load();

private slots:
    void onMoveDrag(Cube::Axis axis, int layer, bool clockwise);
    void onMoveDone(Cube::Axis axis, int layerStart, int layerEnd, int amount);
    void onCubeSolved();

};

#endif // CUBEWIDGET_H
