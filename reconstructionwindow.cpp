#include "reconstructionwindow.h"
#include "ui_reconstructionwindow.h"

ReconstructionWindow::ReconstructionWindow(Reconstruction *reconstruction, Statistics *statistics, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReconstructionWindow)
{
    ui->setupUi(this);

    this->reconstruction = reconstruction;
    this->statistics = statistics;
}

ReconstructionWindow::~ReconstructionWindow()
{
    delete ui;
}
