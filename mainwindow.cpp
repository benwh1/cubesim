#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //create the cube and give it to the CubeGraphicsView
    cube = new Cube();
    ui->cubeWidget->initialize(cube);

    //set the window title
    updateTitleText();

    //detect when the cube size changes so we can update the window title
    connect(cube, SIGNAL(cubeSizeChanged()), this, SLOT(onCubeSizeChanged()));

    //create the settings window and give it the cubeWidget's settings object
    settingsWindow = new SettingsWindow(ui->cubeWidget->getSettings());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete settingsWindow;
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    event->accept();

    if(event->key() == Qt::Key_W){
        settingsWindow->exec();
    }
    else{
        event->ignore();
    }

    if(!event->isAccepted()){
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::updateTitleText(){
    QString size = QString::number(cube->getSize());
    QString str = "cubesim v" + Global::version() + " (" + size + "x" + size + "x" + size + ")";

    setWindowTitle(str);
}

void MainWindow::onCubeSizeChanged(){
    updateTitleText();
}

