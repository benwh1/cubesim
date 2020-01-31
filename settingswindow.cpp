#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(Settings *settings, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    this->settings = settings;

    connect(ui->antialiasingCheckBox, SIGNAL(toggled(bool)), this, SLOT(onAntialiasingCheckBoxChanged()));
    connect(ui->lineWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onLineWidthSpinBoxChanged()));
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::onAntialiasingCheckBoxChanged(){
    settings->setAntialiasing(ui->antialiasingCheckBox->isChecked());
}

void SettingsWindow::onLineWidthSpinBoxChanged(){
    settings->setLineWidth(ui->lineWidthSpinBox->value());
}
