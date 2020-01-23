#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //create the cube and give it to the CubeGraphicsView
    cube = new Cube();
    ui->graphicsView->initialize(cube);

    //set the window title
    updateTitleText();

    //detect when the cube size changes so we can update the window title
    connect(cube, SIGNAL(cubeSizeChanged()), this, SLOT(onCubeSizeChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTitleText(){
    QString size = QString::number(cube->getSize());
    QString str = "cubesim (" + size + "x" + size + "x" + size + ")";

    setWindowTitle(str);
}

void MainWindow::onCubeSizeChanged(){
    updateTitleText();
}

