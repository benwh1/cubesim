#include "replayrecordersettingswidget.h"
#include "ui_replayrecordersettingswidget.h"

ReplayRecorderSettingsWidget::ReplayRecorderSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReplayRecorderSettingsWidget)
{
    ui->setupUi(this);
}

ReplayRecorderSettingsWidget::~ReplayRecorderSettingsWidget()
{
    delete ui;
}
