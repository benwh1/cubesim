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
    connect(ui->extremeFrameDurationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExtremeFrameDurationSpinBoxChanged()));
    connect(ui->videoLengthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onVideoLengthSpinBoxChanged()));
    connect(ui->videoWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onVideoWidthSpinBoxChanged()));
    connect(ui->videoHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onVideoHeightSpinBoxChanged()));
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
                               ui->timePerFrameSpinBox,
                               ui->extremeFrameDurationSpinBox,
                               ui->videoLengthSpinBox,
                               ui->videoWidthSpinBox,
                               ui->videoHeightSpinBox};

    //block signals
    foreach(QWidget *w, widgets){
        w->blockSignals(true);
    }

    //update the UI
    ui->playbackFrameRateSpinBox->setValue(replayRecorderSettings->getPlaybackFrameRate());
    ui->speedSpinBox->setValue(replayRecorderSettings->getSpeed());
    ui->numberOfFramesSpinBox->setValue(replayRecorderSettings->getNumberOfFrames());
    ui->timePerFrameSpinBox->setValue(replayRecorderSettings->getTimePerFrame());
    ui->extremeFrameDurationSpinBox->setValue(replayRecorderSettings->getExtremeFrameDuration());
    ui->videoLengthSpinBox->setValue(replayRecorderSettings->getVideoLength());
    ui->videoWidthSpinBox->setValue(replayRecorderSettings->getVideoWidth());
    ui->videoHeightSpinBox->setValue(replayRecorderSettings->getVideoHeight());

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

void ReplayRecorderSettingsWidget::onExtremeFrameDurationSpinBoxChanged(){
    qreal extremeFrameDuration = ui->extremeFrameDurationSpinBox->value();
    replayRecorderSettings->setExtremeFrameDuration(extremeFrameDuration);
}

void ReplayRecorderSettingsWidget::onVideoLengthSpinBoxChanged(){
    qreal videoLength = ui->videoLengthSpinBox->value();
    replayRecorderSettings->setVideoLength(videoLength);
}

void ReplayRecorderSettingsWidget::onVideoWidthSpinBoxChanged(){
    int videoWidth = ui->videoWidthSpinBox->value();
    replayRecorderSettings->setVideoWidth(videoWidth);
}

void ReplayRecorderSettingsWidget::onVideoHeightSpinBoxChanged(){
    int videoHeight = ui->videoHeightSpinBox->value();
    replayRecorderSettings->setVideoHeight(videoHeight);
}

void ReplayRecorderSettingsWidget::onSettingChanged(){
    synchronizeFromSettings();
}
