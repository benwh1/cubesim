#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QElapsedTimer>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QWidget>
#include "cube.h"
#include "global.h"
#include "reconstruction.h"
#include "saveconverter.h"
#include "settings.h"
#include "statistics.h"

class ReplayRecorder;

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

    Cube *getCube();
    ReplayRecorder *getReplayRecorder();
    Settings *getSettings();
    State getState();

protected:
    void keyPressEvent(QKeyEvent *event);

    void resizeEvent(QResizeEvent *event);

private:
    Ui::CubeWidget *ui;

    Cube *cube;
    Reconstruction *reconstruction;
    ReplayRecorder *replayRecorder;
    Settings *settings;
    Statistics *statistics;

    State state;

    bool swapCtrlShift;
    bool overlapStats;

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

    void save();
    void load();
    void load(QString fileName);

private slots:
    void onMoveDrag(Axis axis, int layer, bool clockwise, Qt::MouseButton button);
    void onMoveDone(Move move);
    void onRotationDone(Move move);
    void onCubeSolved();

};

#endif // CUBEWIDGET_H
