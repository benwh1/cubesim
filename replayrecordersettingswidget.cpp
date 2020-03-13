#include "replayrecordersettingswidget.h"
#include "ui_replayrecordersettingswidget.h"

ReplayRecorderSettingsWidget::ReplayRecorderSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReplayRecorderSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->playbackFrameRateSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onPlaybackFrameRateSpinBoxChanged()));
    connect(ui->speedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onSpeedSpinBoxChanged()));
    connect(ui->numberOfFramesSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onNumberOfFramesSpinBoxChanged()));
    connect(ui->timePerFrameSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onTimePerFrameSpinBoxChanged()));
}

ReplayRecorderSettingsWidget::~ReplayRecorderSettingsWidget()
{
    delete ui;
}

void ReplayRecorderSettingsWidget::initialize(ReplayRecorderSettings *replayRecorderSettings){
    this->replayRecorderSettings = replayRecorderSettings;

    //when a setting is changed, update the UI
    connect(replayRecorderSettings, SIGNAL(settingChanged()), this, SLOT(onSettingChanged()));

    //update the UI to make sure it matches the initial settings
    synchronizeFromSettings();
}

void ReplayRecorderSettingsWidget::synchronizeFromSettings(){
    //we need to block all signals so that updating the UI doesn't cause
    //signals to be emitted which would in turn call this function and create
    //an infinite loop

    //list of all settings widgets
    QList<QWidget*> widgets = {ui->playbackFrameRateSpinBox,
                               ui->speedSpinBox,
                               ui->numberOfFramesSpinBox,
                               ui->timePerFrameSpinBox};

    //block signals
    foreach(QWidget *w, widgets){
        w->blockSignals(true);
    }

    //update the UI
    ui->playbackFrameRateSpinBox->setValue(replayRecorderSettings->getPlaybackFrameRate());
    ui->speedSpinBox->setValue(replayRecorderSettings->getSpeed());
    ui->numberOfFramesSpinBox->setValue(replayRecorderSettings->getNumberOfFrames());
    ui->timePerFrameSpinBox->setValue(replayRecorderSettings->getTimePerFrame());

    //unblock signals
    foreach(QWidget *w, widgets){
        w->blockSignals(false);
    }
}

void ReplayRecorderSettingsWidget::onPlaybackFrameRateSpinBoxChanged(){
    int playbackFrameRate = ui->playbackFrameRateSpinBox->value();
    replayRecorderSettings->setPlaybackFrameRate(playbackFrameRate);
}

void ReplayRecorderSettingsWidget::onSpeedSpinBoxChanged(){
    qreal speed = ui->speedSpinBox->value();
    replayRecorderSettings->setSpeed(speed);
}

void ReplayRecorderSettingsWidget::onNumberOfFramesSpinBoxChanged(){
    int numberOfFrames = ui->numberOfFramesSpinBox->value();
    replayRecorderSettings->setNumberOfFrames(numberOfFrames);
}

void ReplayRecorderSettingsWidget::onTimePerFrameSpinBoxChanged(){
    qreal timePerFrame = ui->timePerFrameSpinBox->value();
    replayRecorderSettings->setTimePerFrame(timePerFrame);
}

void ReplayRecorderSettingsWidget::onSettingChanged(){
    synchronizeFromSettings();
}
