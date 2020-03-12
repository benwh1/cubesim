#ifndef REPLAYRECORDERWINDOW_H
#define REPLAYRECORDERWINDOW_H

#include <QDialog>
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

private:
    Ui::ReplayRecorderWindow *ui;

    ReplayRecorder *replayRecorder;

private slots:
    void onRenderButtonClicked();

};

#endif // REPLAYRECORDERWINDOW_H
