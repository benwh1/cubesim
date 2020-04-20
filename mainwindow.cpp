#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set the window title
    updateTitleText();

    //detect when the cube size changes so we can update the window title
    connect(ui->cubeWidget->getCube(), SIGNAL(cubeSizeChanged()), this, SLOT(onCubeSizeChanged()));

    //create windows
    reconstructionWindow = new ReconstructionWindow(ui->cubeWidget->getReconstruction(), ui->cubeWidget->getStatistics());
    replayRecorderWindow = new ReplayRecorderWindow(ui->cubeWidget->getReplayRecorder());
    settingsWindow = new SettingsWindow(ui->cubeWidget->getSettings());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete reconstructionWindow;
    delete replayRecorderWindow;
    delete settingsWindow;
}

void MainWindow::closeEvent(QCloseEvent *){
    //quit the program when the main window is closed. we need to do this
    //because otherwise it is possible for the program to remain alive but
    //all of the windows to be hidden (e.g. if we close the window while we
    //are recording a replay)
    exit(0);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    event->accept();

    if(event->key() == Qt::Key_W){
        if(ui->cubeWidget->getState() == CubeWidget::State::Neutral ||
           ui->cubeWidget->getState() == CubeWidget::State::Finished){
            settingsWindow->show();

            //save the settings to a file
            QJsonObject data = ui->cubeWidget->getSettings()->toJSON();
            QJsonDocument document(data);

            QFile f("settings.dat");
            f.open(QFile::WriteOnly);
            f.write(document.toBinaryData());
            f.close();
        }
    }
    else if(event->key() == Qt::Key_R){
        if(ui->cubeWidget->getState() == CubeWidget::State::Finished){
            //check whether ffmpeg is installed
            if(QStandardPaths::findExecutable("ffmpeg") == ""){
                QMessageBox::warning(this, "Error", "FFmpeg is not installed");
            }
            else{
                replayRecorderWindow->show();
            }
        }
    }
    else if(event->key() == Qt::Key_L){
        if(ui->cubeWidget->getState() == CubeWidget::State::Finished){
            reconstructionWindow->show();
        }
    }
    else{
        event->ignore();
    }

    if(!event->isAccepted()){
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::updateTitleText(){
    QString size = QString::number(ui->cubeWidget->getCube()->getSize());
    QString str = "cubesim v" + Global::version() + " (" + size + "x" + size + "x" + size + ")";

    setWindowTitle(str);
}

void MainWindow::onCubeSizeChanged(){
    updateTitleText();
}
