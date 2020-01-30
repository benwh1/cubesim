#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include "settings.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(Settings *settings, QWidget *parent = nullptr);
    ~SettingsWindow();

private:
    Ui::SettingsWindow *ui;

    Settings *settings;

private slots:
    void onAntialiasingCheckBoxChanged();
    void onLineWidthSpinBoxChanged();

};

#endif // SETTINGSWINDOW_H
