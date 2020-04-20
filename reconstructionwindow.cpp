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

void ReconstructionWindow::show(){
    updateStatistics();
    QDialog::show();
}

void ReconstructionWindow::updateStatistics(){
    int n = statistics->getCubeSize();
    ui->cubeLabel->setText(QString::number(n) + "x" + QString::number(n) + "x" + QString::number(n));
    ui->timeLabel->setText(statistics->timeString());
    ui->movesLabel->setText(statistics->movesString());
    ui->tpsLabel->setText(statistics->tpsString());
    ui->piecesLabel->setText(statistics->numberOfPiecesString());
    ui->piecesPerSecLabel->setText(statistics->piecesPerSecondString());
    ui->timePerPieceLabel->setText(statistics->timePerPieceString());
    ui->piecesPerMoveLabel->setText(statistics->piecesPerMoveString());
    ui->movesPerPieceLabel->setText(statistics->movesPerPieceString());

    ui->reconstructionTextEdit->setText(reconstruction->toString());
}
