#ifndef RECONSTRUCTIONWINDOW_H
#define RECONSTRUCTIONWINDOW_H

#include <QDialog>

namespace Ui {
class ReconstructionWindow;
}

class ReconstructionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReconstructionWindow(QWidget *parent = nullptr);
    ~ReconstructionWindow();

private:
    Ui::ReconstructionWindow *ui;
};

#endif // RECONSTRUCTIONWINDOW_H
