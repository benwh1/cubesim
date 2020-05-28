#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>
#include <QStandardPaths>
#include "cube.h"
#include "reconstructionwindow.h"
#include "replayrecorderwindow.h"
#include "settingswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;

    ReconstructionWindow *reconstructionWindow;
    ReplayRecorderWindow *replayRecorderWindow;
    SettingsWindow *settingsWindow;

    void updateTitleText();

private slots:
    void onCubeSizeChanged();

    void onSettingsWindowShortcutActivated();
    void onReplayRecorderWindowShortcutActivated();
    void onReconstructionWindowShortcutActivated();

};
#endif // MAINWINDOW_H
