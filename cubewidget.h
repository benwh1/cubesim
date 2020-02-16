#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QElapsedTimer>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QWidget>
#include "cubestate.h"
#include "global.h"
#include "reconstruction.h"
#include "saveconverter.h"
#include "settings.h"
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

    void initialize(CubeState *cube);

    Settings *getSettings();
    State getState();

protected:
    void keyPressEvent(QKeyEvent *event);

    void resizeEvent(QResizeEvent *event);

private:
    Ui::CubeWidget *ui;

    CubeState *cube;
    Reconstruction reconstruction;
    Statistics *statistics;

    Settings *settings;

    State state;

    bool swapCtrlShift;

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

    void save();
    void load();

private slots:
    void onMoveDrag(Axis axis, int layer, bool clockwise, Qt::MouseButton button);
    void onMoveDone(Axis axis, int layerStart, int layerEnd, int amount);
    void onRotationDone(Axis axis, int amount);
    void onCubeSolved();

};

#endif // CUBEWIDGET_H
