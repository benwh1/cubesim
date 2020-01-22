#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Cube *c = new Cube();
    ui->graphicsView->initialize(c);
}

MainWindow::~MainWindow()
{
    delete ui;
}

