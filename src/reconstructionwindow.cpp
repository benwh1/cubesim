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
    ui->cubeLabel->setText(statistics->cubeSizeString());
    ui->timeLabel->setText(statistics->timeString());
    ui->movesLabel->setText(statistics->movesString());

    Reconstruction *r = reconstruction->simplified();
    int moves = reconstruction->numMoves();
    int movesNoMistakes = r->numMoves();
    qreal mistakesFrac = 1 - (qreal)movesNoMistakes/moves;
    ui->movesNoMistakesLabel->setText(Formatting::formatMovecount(movesNoMistakes));
    ui->mistakesLabel->setText(QString::number(mistakesFrac*100, 'f', 2) + "%");
    delete r;

    ui->rotationsLabel->setText(Formatting::formatMovecount(reconstruction->numRotations()));
    ui->tpsLabel->setText(statistics->tpsString());
    ui->piecesLabel->setText(statistics->numberOfPiecesString());
    ui->piecesPerSecLabel->setText(statistics->piecesPerSecondString());
    ui->timePerPieceLabel->setText(statistics->timePerPieceString());
    ui->piecesPerMoveLabel->setText(statistics->piecesPerMoveString());
    ui->movesPerPieceLabel->setText(statistics->movesPerPieceString());

    ui->reconstructionTextEdit->setText(reconstruction->toString());
}
