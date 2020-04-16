#include "replayrecorderwindow.h"
#include "ui_replayrecorderwindow.h"

ReplayRecorderWindow::ReplayRecorderWindow(ReplayRecorder *replayRecorder, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplayRecorderWindow)
{
    ui->setupUi(this);

    this->replayRecorder = replayRecorder;

    //pass the ReplayRecorderSettings object to the ReplayRecorderSettingsWidget
    ui->replayRecorderSettingsWidget->initialize(replayRecorder->getSettings());

    //make the progress bar and abort button invisible initially
    ui->progressBar->setVisible(false);
    ui->abortButton->setVisible(false);

    //resize the window
    shrinkWindow();

    //connect the render and abort buttons
    connect(ui->renderButton, SIGNAL(clicked(bool)), this, SLOT(onRenderButtonClicked()));
    connect(ui->abortButton, SIGNAL(clicked(bool)), this, SLOT(onAbortButtonClicked()));

    //signals from replayRecorder
    connect(replayRecorder, SIGNAL(frameRendered(int,int)), this, SLOT(onFrameRendered(int,int)));
    connect(replayRecorder, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
    connect(replayRecorder, SIGNAL(stateChanged()), this, SLOT(onStateChanged()));
}

ReplayRecorderWindow::~ReplayRecorderWindow()
{
    delete ui;
}

void ReplayRecorderWindow::open(){
    replayRecorder->getSettings()->reset();
    QDialog::open();
}

void ReplayRecorderWindow::shrinkWindow(){
    //process events so the minimumSize of the window is computed correctly
    //e.g. need to process events after setting visibility of progress bar
    QApplication::processEvents();

    //resize the window
    resize(minimumSize());

    //prevent the window from being resized
    setMaximumSize(minimumSize());
}

void ReplayRecorderWindow::onRenderButtonClicked(){
    replayRecorder->record();
}

void ReplayRecorderWindow::onAbortButtonClicked(){
    replayRecorder->abort();
}

void ReplayRecorderWindow::onFrameRendered(int frame, int total){
    //current progress = frame / total
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(frame);

    //process events, so the progress bar gets updated properly
    QApplication::processEvents();

    //check if this was the last frame
    if(frame == total){
        //hide the progress bar
        ui->progressBar->setVisible(false);

        //show the render button and hide the abort button
        ui->renderButton->setVisible(true);
        ui->abortButton->setVisible(false);

        //resize the window
        shrinkWindow();
    }
}

void ReplayRecorderWindow::onFinished(int returnCode){
    if(returnCode == 0){
        QMessageBox::information(this, "Replay Recorder", "Video rendering finished");
    }
    else{
        QMessageBox::warning(this, "Replay Recorder", "Video rendering failed");
    }
}

void ReplayRecorderWindow::onStateChanged(){
    ReplayRecorder::State state = replayRecorder->getState();

    if(state == ReplayRecorder::State::Neutral){
        ui->abortButton->setVisible(false);
        ui->renderButton->setVisible(true);
        ui->progressBar->setVisible(false);
    }
    else if(state == ReplayRecorder::State::Recording){
        ui->abortButton->setVisible(true);
        ui->renderButton->setVisible(false);
        ui->progressBar->setVisible(true);
        ui->progressBar->setValue(0);
    }
    else if(state == ReplayRecorder::State::WaitingForFFmpeg){
        ui->abortButton->setVisible(false);
        ui->renderButton->setVisible(false);
        ui->progressBar->setVisible(false);
    }

    //resize the window
    shrinkWindow();
}
