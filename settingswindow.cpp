#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(Settings *settings, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    this->settings = settings;

    connect(ui->antialiasingCheckBox, SIGNAL(toggled(bool)), this, SLOT(onAntialiasingCheckBoxChanged()));
    connect(ui->backgroundColourWidget, SIGNAL(colorChanged()), this, SLOT(onBackgroundColourWidgetChanged()));
    connect(ui->lineColourWidget, SIGNAL(colorChanged()), this, SLOT(onLineColourWidgetChanged()));
    connect(ui->lineWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onLineWidthSpinBoxChanged()));
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::onAntialiasingCheckBoxChanged(){
    settings->setAntialiasing(ui->antialiasingCheckBox->isChecked());
}

void SettingsWindow::onBackgroundColourWidgetChanged(){
    settings->setBackgroundColour(ui->backgroundColourWidget->getColor());
}

void SettingsWindow::onLineColourWidgetChanged(){
    settings->setLineColour(ui->lineColourWidget->getColor());
}

void SettingsWindow::onLineWidthSpinBoxChanged(){
    settings->setLineWidth(ui->lineWidthSpinBox->value());
}
