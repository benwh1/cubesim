#include "replayrecorderwindow.h"
#include "ui_replayrecorderwindow.h"

ReplayRecorderWindow::ReplayRecorderWindow(ReplayRecorder *replayRecorder, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplayRecorderWindow)
{
    ui->setupUi(this);

    this->replayRecorder = replayRecorder;

    //connect the render button
    connect(ui->renderButton, SIGNAL(clicked(bool)), this, SLOT(onRenderButtonClicked()));
}

ReplayRecorderWindow::~ReplayRecorderWindow()
{
    delete ui;
}

void ReplayRecorderWindow::onRenderButtonClicked(){
    int frameRate = ui->playbackFrameRateSpinBox->value();
    qreal speed = ui->speedSpinBox->value();

    replayRecorder->record(frameRate, speed);
}
