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

private slots:
    void onAntialiasingCheckBoxChanged();
    void onLineWidthSpinBoxChanged();

};

#endif // SETTINGSWINDOW_H
