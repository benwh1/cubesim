#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QWidget>
#include "cube.h"
#include "version.h"
#include "reconstruction.h"
#include "saveconverter.h"
#include "settings.h"
#include "statistics.h"
#include "timedbool.h"

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
    Reconstruction *getReconstruction();
    ReplayRecorder *getReplayRecorder();
    Settings *getSettings();
    State getState();
    Statistics *getStatistics();

    void reset();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::CubeWidget *ui;

    Cube *cube;
    Reconstruction *reconstruction;
    ReplayRecorder *replayRecorder;
    Settings *settings;
    Statistics *statistics;

    State state;

    TimedBool canDoMove;

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

    void save();
    void save(QString fileName);
    void load();
    void load(QString fileName);

    void setState(State s);

private slots:
    void onMoveDrag(Axis axis, int layer, bool clockwise, Qt::MouseButton button);
    void onMoveDone();
    void onCubeSolved();

    //controls
    void onScrambleShortcutActivated();
    void onResetShortcutActivated();
    void onIncreaseSizeShortcutActivated();
    void onDecreaseSizeShortcutActivated();
    void onChangeSizeShortcutActivated();

    void onLoadProjectionShortcutActivated(int i);
    void onResetProjectionShortcutActivated();
    void onChangeProjectionShortcutActivated();

    void onResetZoomShortcutActivated();
    void onZoomInShortcutActivated();
    void onZoomInSmallShortcutActivated();
    void onZoomInLargeShortcutActivated();
    void onZoomOutShortcutActivated();
    void onZoomOutSmallShortcutActivated();
    void onZoomOutLargeShortcutActivated();

    void onSaveShortcutActivated();
    void onLoadShortcutActivated();

    void onToggleStatsShortcutActivated();

    void onToggleMultisliceShortcutActivated();

    void onScreenshotShortcutActivated();

    void onMinMoveDurationChanged();

    //replay recorder
    void onReplayRecorderSettingChanged();

signals:
    void solveFinished();
    void stateChanged();

};

#endif // CUBEWIDGET_H
