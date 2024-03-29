#ifndef CONTROLSWIDGET_H
#define CONTROLSWIDGET_H

#include <QWidget>
#include "controls.h"

namespace Ui {
class ControlsWidget;
}

class ControlsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlsWidget(QWidget *parent = nullptr);
    ~ControlsWidget();

    void initialize(Controls *controls);

    //make the UI match the controls in the controls object
    void synchronizeFromControls();

private:
    Ui::ControlsWidget *ui;

    bool initialized;

    Controls *controls;

private slots:
    void onScrambleKeySequenceChanged();
    void onResetKeySequenceChanged();
    void onIncreaseSizeKeySequenceChanged();
    void onDecreaseSizeKeySequenceChanged();
    void onChangeSizeKeySequenceChanged();

    void onLoadProjection1KeySequenceChanged();
    void onLoadProjection2KeySequenceChanged();
    void onLoadProjection3KeySequenceChanged();
    void onLoadProjection4KeySequenceChanged();
    void onLoadProjection5KeySequenceChanged();
    void onLoadProjection6KeySequenceChanged();
    void onLoadProjection7KeySequenceChanged();
    void onLoadProjection8KeySequenceChanged();
    void onLoadProjection9KeySequenceChanged();
    void onLoadProjection10KeySequenceChanged();
    void onResetProjectionKeySequenceChanged();
    void onChangeProjectionKeySequenceChanged();

    void onResetZoomKeySequenceChanged();
    void onZoomInKeySequenceChanged();
    void onZoomInSmallKeySequenceChanged();
    void onZoomInLargeKeySequenceChanged();
    void onZoomOutKeySequenceChanged();
    void onZoomOutSmallKeySequenceChanged();
    void onZoomOutLargeKeySequenceChanged();

    void onSaveKeySequenceChanged();
    void onLoadKeySequenceChanged();

    void onToggleStatsKeySequenceChanged();

    void onToggleMultisliceKeySequenceChanged();

    void onScreenshotKeySequenceChanged();

    void onSettingsWindowKeySequenceChanged();
    void onReplayRecorderWindowKeySequenceChanged();
    void onReconstructionWindowKeySequenceChanged();

    void onLeftClickComboBoxChanged();
    void onCtrlClickComboBoxChanged();
    void onShiftClickComboBoxChanged();
    void onRightClickComboBoxChanged();
    void onMiddleClickComboBoxChanged();

    void onMinMoveDurationSpinBoxChanged();

};

#endif // CONTROLSWIDGET_H
