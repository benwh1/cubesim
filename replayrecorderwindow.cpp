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

    //make the progress bar invisible initially, and resize the window
    ui->progressBar->setVisible(false);
    shrinkWindow();

    //connect the render button
    connect(ui->renderButton, SIGNAL(clicked(bool)), this, SLOT(onRenderButtonClicked()));

    //detect when a frame has been rendered
    connect(replayRecorder, SIGNAL(frameRendered(int,int)), this, SLOT(onFrameRendered(int,int)));
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
    //reset the progress bar and make it visible
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);

    //resize the window
    shrinkWindow();

    //record the replay
    replayRecorder->record();
}

void ReplayRecorderWindow::onFrameRendered(int frame, int total){
    //current progress = frame / total
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(frame);

    //process events, so the progress bar gets updated properly
    QApplication::processEvents();

    //check if this was the last frame. if so, hide the progress bar
    if(frame == total){
        ui->progressBar->setVisible(false);

        //resize the window
        shrinkWindow();
    }
}
