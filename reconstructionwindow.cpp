#include "reconstructionwindow.h"
#include "ui_reconstructionwindow.h"

ReconstructionWindow::ReconstructionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReconstructionWindow)
{
    ui->setupUi(this);
}

ReconstructionWindow::~ReconstructionWindow()
{
    delete ui;
}
