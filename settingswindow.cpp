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
    connect(ui->borderPaddingSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onBorderPaddingSpinBoxChanged()));
    connect(ui->faceUColourWidget, SIGNAL(colorChanged()), this, SLOT(onFaceUColourChanged()));
    connect(ui->faceFColourWidget, SIGNAL(colorChanged()), this, SLOT(onFaceFColourChanged()));
    connect(ui->faceRColourWidget, SIGNAL(colorChanged()), this, SLOT(onFaceRColourChanged()));
    connect(ui->faceBColourWidget, SIGNAL(colorChanged()), this, SLOT(onFaceBColourChanged()));
    connect(ui->faceLColourWidget, SIGNAL(colorChanged()), this, SLOT(onFaceLColourChanged()));
    connect(ui->faceDColourWidget, SIGNAL(colorChanged()), this, SLOT(onFaceDColourChanged()));
    connect(ui->guideLinesCrossCheckBox, SIGNAL(toggled(bool)), this, SLOT(onGuideLinesCrossCheckBoxChanged()));
    connect(ui->guideLinesPlusCheckBox, SIGNAL(toggled(bool)), this, SLOT(onGuideLinesPlusCheckBoxChanged()));
    connect(ui->guideLinesBoxCheckBox, SIGNAL(toggled(bool)), this, SLOT(onGuideLinesBoxCheckBoxChanged()));
    connect(ui->guideLineColourWidget, SIGNAL(colorChanged()), this, SLOT(onGuideLineColourWidgetChanged()));
    connect(ui->guideLineWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onGuideLineWidthSpinBoxChanged()));
    connect(ui->supercubeCheckBox, SIGNAL(toggled(bool)), this, SLOT(onSupercubeCheckBoxChanged()));
    connect(ui->supercubeStickersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSupercubeStickersComboBoxChanged()));
    connect(ui->pochmannBarThicknessSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onPochmannBarThicknessChanged()));
    connect(ui->pochmannCageCheckBox, SIGNAL(toggled(bool)), this, SLOT(onPochmannCageChanged()));

    connect(settings, SIGNAL(settingChanged()), this, SLOT(onSettingChanged()));

    connect(this, SIGNAL(finished(int)), this, SLOT(onFinished()));

    //pass the settings' Controls object to the ControlsWidget
    ui->controlsWidget->initialize(settings->getControls());

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
                              ui->borderPaddingSpinBox,
                              ui->faceUColourWidget,
                              ui->faceFColourWidget,
                              ui->faceRColourWidget,
                              ui->faceBColourWidget,
                              ui->faceLColourWidget,
                              ui->faceDColourWidget,
                              ui->guideLinesCrossCheckBox,
                              ui->guideLinesPlusCheckBox,
                              ui->guideLinesBoxCheckBox,
                              ui->guideLineColourWidget,
                              ui->guideLineWidthSpinBox,
                              ui->supercubeCheckBox,
                              ui->supercubeStickersComboBox,
                              ui->pochmannBarThicknessSpinBox,
                              ui->pochmannCageCheckBox};

    foreach(QWidget *w, widgets){
        w->blockSignals(true);
    }

    ui->antialiasingCheckBox->setChecked(settings->getAntialiasing());
    ui->backgroundColourWidget->setColor(settings->getBackgroundColour());
    ui->lineColourWidget->setColor(settings->getLineColour());
    ui->lineWidthSpinBox->setValue(settings->getLineWidth());
    ui->borderPaddingSpinBox->setValue(settings->getBorderPadding());
    ui->faceUColourWidget->setColor(settings->getColour(Face::U));
    ui->faceFColourWidget->setColor(settings->getColour(Face::F));
    ui->faceRColourWidget->setColor(settings->getColour(Face::R));
    ui->faceBColourWidget->setColor(settings->getColour(Face::B));
    ui->faceLColourWidget->setColor(settings->getColour(Face::L));
    ui->faceDColourWidget->setColor(settings->getColour(Face::D));
    ui->guideLinesCrossCheckBox->setChecked(settings->getGuideLinesCross());
    ui->guideLinesPlusCheckBox->setChecked(settings->getGuideLinesPlus());
    ui->guideLinesBoxCheckBox->setChecked(settings->getGuideLinesBox());
    ui->guideLineColourWidget->setColor(settings->getGuideLineColour());
    ui->guideLineWidthSpinBox->setValue(settings->getGuideLineWidth());
    ui->supercubeCheckBox->setChecked(settings->getSupercube());
    ui->supercubeStickersComboBox->setCurrentText(settings->getSupercubeStickers());
    ui->pochmannBarThicknessSpinBox->setValue(settings->getPochmannBarThickness());
    ui->pochmannCageCheckBox->setChecked(settings->getPochmannCage());

    ui->controlsWidget->synchronizeFromControls();

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

void SettingsWindow::onBorderPaddingSpinBoxChanged(){
    settings->setBorderPadding(ui->borderPaddingSpinBox->value());
}

void SettingsWindow::onFaceUColourChanged(){
    settings->setColour(Face::U, ui->faceUColourWidget->getColor());
}

void SettingsWindow::onFaceFColourChanged(){
    settings->setColour(Face::F, ui->faceFColourWidget->getColor());
}

void SettingsWindow::onFaceRColourChanged(){
    settings->setColour(Face::R, ui->faceRColourWidget->getColor());
}

void SettingsWindow::onFaceBColourChanged(){
    settings->setColour(Face::B, ui->faceBColourWidget->getColor());
}

void SettingsWindow::onFaceLColourChanged(){
    settings->setColour(Face::L, ui->faceLColourWidget->getColor());
}

void SettingsWindow::onFaceDColourChanged(){
    settings->setColour(Face::D, ui->faceDColourWidget->getColor());
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

void SettingsWindow::onSupercubeCheckBoxChanged(){
    settings->setSupercube(ui->supercubeCheckBox->isChecked());
}

void SettingsWindow::onSupercubeStickersComboBoxChanged(){
    settings->setSupercubeStickers(ui->supercubeStickersComboBox->currentText());
}

void SettingsWindow::onPochmannBarThicknessChanged(){
    settings->setPochmannBarThickness(ui->pochmannBarThicknessSpinBox->value());
}

void SettingsWindow::onPochmannCageChanged(){
    settings->setPochmannCage(ui->pochmannCageCheckBox->isChecked());
}

void SettingsWindow::onSettingChanged(){
    synchronizeFromSettings();
}

void SettingsWindow::onFinished(){
    settings->save("settings.dat");
}
