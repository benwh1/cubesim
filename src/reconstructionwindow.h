#ifndef RECONSTRUCTIONWINDOW_H
#define RECONSTRUCTIONWINDOW_H

#include <QDialog>
#include "reconstruction.h"
#include "statistics.h"

namespace Ui {
class ReconstructionWindow;
}

class ReconstructionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReconstructionWindow(Reconstruction *reconstruction, Statistics *statistics, QWidget *parent = nullptr);
    ~ReconstructionWindow();

public slots:
    void show();

private:
    Ui::ReconstructionWindow *ui;

    Reconstruction *reconstruction;
    Statistics *statistics;

    void updateStatistics();

};

#endif // RECONSTRUCTIONWINDOW_H
