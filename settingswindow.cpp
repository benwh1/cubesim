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
    connect(ui->multisliceCheckBox, SIGNAL(toggled(bool)), this, SLOT(onMultisliceCheckBoxChanged()));
    connect(ui->guideLinesCrossCheckBox, SIGNAL(toggled(bool)), this, SLOT(onGuideLinesCrossCheckBoxChanged()));
    connect(ui->guideLinesPlusCheckBox, SIGNAL(toggled(bool)), this, SLOT(onGuideLinesPlusCheckBoxChanged()));
    connect(ui->guideLinesBoxCheckBox, SIGNAL(toggled(bool)), this, SLOT(onGuideLinesBoxCheckBoxChanged()));
    connect(ui->guideLineColourWidget, SIGNAL(colorChanged()), this, SLOT(onGuideLineColourWidgetChanged()));
    connect(ui->guideLineWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onGuideLineWidthSpinBoxChanged()));

    connect(settings, SIGNAL(settingChanged()), this, SLOT(onSettingChanged()));

    synchronizeFromSettings();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::synchronizeFromSettings(){
    //we need to block all the signals here so that changing the widgets
    //in the UI doesn't cause a signal to get emitted, causing the setting
    //to change, causing settingChanged to be emitted, causing
    //synchronizeFromSettings to be called, and creating an infinite loop

    QList<QWidget*> widgets = {ui->antialiasingCheckBox,
                              ui->backgroundColourWidget,
                              ui->lineColourWidget,
                              ui->lineWidthSpinBox,
                              ui->multisliceCheckBox,
                              ui->guideLinesCrossCheckBox,
                              ui->guideLinesPlusCheckBox,
                              ui->guideLinesBoxCheckBox,
                              ui->guideLineColourWidget,
                              ui->guideLineWidthSpinBox};

    foreach(QWidget *w, widgets){
        w->blockSignals(true);
    }

    ui->antialiasingCheckBox->setChecked(settings->getAntialiasing());
    ui->backgroundColourWidget->setColor(settings->getBackgroundColour());
    ui->lineColourWidget->setColor(settings->getLineColour());
    ui->lineWidthSpinBox->setValue(settings->getLineWidth());
    ui->multisliceCheckBox->setChecked(settings->getMultislice());
    ui->guideLinesCrossCheckBox->setChecked(settings->getGuideLinesCross());
    ui->guideLinesPlusCheckBox->setChecked(settings->getGuideLinesPlus());
    ui->guideLinesBoxCheckBox->setChecked(settings->getGuideLinesBox());
    ui->guideLineColourWidget->setColor(settings->getGuideLineColour());
    ui->guideLineWidthSpinBox->setValue(settings->getGuideLineWidth());

    foreach(QWidget *w, widgets){
        w->blockSignals(false);
    }
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

void SettingsWindow::onMultisliceCheckBoxChanged(){
    settings->setMultislice(ui->multisliceCheckBox->isChecked());
}

void SettingsWindow::onGuideLinesCrossCheckBoxChanged(){
    settings->setGuideLinesCross(ui->guideLinesCrossCheckBox->isChecked());
}

void SettingsWindow::onGuideLinesPlusCheckBoxChanged(){
    settings->setGuideLinesPlus(ui->guideLinesPlusCheckBox->isChecked());
}

void SettingsWindow::onGuideLinesBoxCheckBoxChanged(){
    settings->setGuideLinesBox(ui->guideLinesBoxCheckBox->isChecked());
}

void SettingsWindow::onGuideLineColourWidgetChanged(){
    settings->setGuideLineColour(ui->guideLineColourWidget->getColor());
}

void SettingsWindow::onGuideLineWidthSpinBoxChanged(){
    settings->setGuideLineWidth(ui->guideLineWidthSpinBox->value());
}

void SettingsWindow::onSettingChanged(){
    synchronizeFromSettings();
}
