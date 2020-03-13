#ifndef REPLAYRECORDERSETTINGSWIDGET_H
#define REPLAYRECORDERSETTINGSWIDGET_H

#include <QWidget>
#include "replayrecordersettings.h"

namespace Ui {
class ReplayRecorderSettingsWidget;
}

class ReplayRecorderSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReplayRecorderSettingsWidget(QWidget *parent = nullptr);
    ~ReplayRecorderSettingsWidget();

    void initialize(ReplayRecorderSettings *replayRecorderSettings);

private:
    Ui::ReplayRecorderSettingsWidget *ui;

    ReplayRecorderSettings *replayRecorderSettings;

    //make the UI match the settings in the replayRecorderSettings object
    void synchronizeFromSettings();

private slots:
    void onPlaybackFrameRateSpinBoxChanged();
    void onSpeedSpinBoxChanged();
    void onNumberOfFramesSpinBoxChanged();
    void onTimePerFrameSpinBoxChanged();
    void onExtremeFrameDurationSpinBoxChanged();
    void onVideoLengthSpinBoxChanged();

    void onSettingChanged();

};

#endif // REPLAYRECORDERSETTINGSWIDGET_H
