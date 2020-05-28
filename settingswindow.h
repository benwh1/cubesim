#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include "settings.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(Settings *settings, QDialog *parent = nullptr);
    ~SettingsWindow();

private:
    Ui::SettingsWindow *ui;

    Settings *settings;

    //make the UI match the settings in the settings object
    void synchronizeFromSettings();

private slots:
    void onAntialiasingCheckBoxChanged();
    void onBackgroundColourWidgetChanged();
    void onLineColourWidgetChanged();
    void onLineWidthSpinBoxChanged();
    void onFaceUColourChanged();
    void onFaceFColourChanged();
    void onFaceRColourChanged();
    void onFaceBColourChanged();
    void onFaceLColourChanged();
    void onFaceDColourChanged();
    void onGuideLinesCrossCheckBoxChanged();
    void onGuideLinesPlusCheckBoxChanged();
    void onGuideLinesBoxCheckBoxChanged();
    void onGuideLineColourWidgetChanged();
    void onGuideLineWidthSpinBoxChanged();
    void onSupercubeCheckBoxChanged();
    void onSupercubeStickersComboBoxChanged();
    void onPochmannBarThicknessChanged();
    void onPochmannCageChanged();

    void onSettingChanged();

    void onFinished();

};

#endif // SETTINGSWINDOW_H
