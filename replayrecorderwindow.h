#ifndef REPLAYRECORDERWINDOW_H
#define REPLAYRECORDERWINDOW_H

#include <QApplication>
#include <QDialog>
#include <QMessageBox>
#include "replayrecorder.h"

namespace Ui {
class ReplayRecorderWindow;
}

class ReplayRecorderWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReplayRecorderWindow(ReplayRecorder *replayRecorder, QWidget *parent = nullptr);
    ~ReplayRecorderWindow();

public slots:
    void show();

private:
    Ui::ReplayRecorderWindow *ui;

    ReplayRecorder *replayRecorder;

    void shrinkWindow();

private slots:
    void onRenderButtonClicked();
    void onAbortButtonClicked();

    void onFrameRendered(int frame, int total);
    void onFinished(int returnCode);
    void onStateChanged();
    void onAborted();

};

#endif // REPLAYRECORDERWINDOW_H
