#include "statisticswidget.h"
#include "ui_statisticswidget.h"

StatisticsWidget::StatisticsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWidget)
{
    ui->setupUi(this);

    initialized = false;

    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

StatisticsWidget::~StatisticsWidget()
{
    delete ui;
}

void StatisticsWidget::initialize(Statistics *statistics, Settings *settings){
    this->statistics = statistics;

    connect(statistics, SIGNAL(timerStarted()), this, SLOT(onTimerStarted()));

    //when the timer is stopped, we need to make sure that the displayed time
    //is exactly the final time, because it may be a few ms off from the last
    //time it was updated
    connect(statistics, SIGNAL(timerStopped()), this, SLOT(onTimerStopped()));
    connect(statistics, SIGNAL(timerReset()), this, SLOT(onTimerReset()));

    //update the UI whenever a statistic is changed
    connect(statistics, SIGNAL(statisticChanged()), this, SLOT(onStatisticChanged()));

    this->settings = settings;

    //connect to signals that are emitted from settings being changed
    connect(settings, SIGNAL(backgroundColourChanged()), this, SLOT(onBackgroundColourSettingChanged()));

    initialized = true;
}

QJsonObject StatisticsWidget::toJSON(){
    QJsonObject data;

    data["timeStyleSheet"] = ui->timeLabel->styleSheet();

    return data;
}

void StatisticsWidget::fromJSON(QJsonObject data){
    ui->timeLabel->setStyleSheet(data["timeStyleSheet"].toString());

    //update the widget. we need to do this because if the timer in
    //statistics is not running, then the timeLabel won't show the
    //correct time
    updateStatistics();
}

void StatisticsWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setBrush(QBrush(settings->getBackgroundColour()));
    painter.drawRect(0, 0, width()-1, height()-1);
    QWidget::paintEvent(event);
}

void StatisticsWidget::updateStatistics(bool force){
    //make sure we actually have a statistics object to work with
    if(!initialized) return;

    if(statistics->timerRunning() || force){
        ui->timeLabel->setText(statistics->timeString());
        ui->movesLabel->setText(statistics->movesString());
        ui->tpsLabel->setText(statistics->tpsString());
    }
}

void StatisticsWidget::clear(){
    ui->timeLabel->setText("");
    ui->movesLabel->setText("");
    ui->tpsLabel->setText("");

    ui->timeLabel->setStyleSheet("");
}

void StatisticsWidget::onTimeout(){
    updateStatistics(false);
}

void StatisticsWidget::onTimerStarted(){
    ui->timeLabel->setStyleSheet("");

    updateTimer.start(30);
    updateStatistics(false);
}

void StatisticsWidget::onTimerStopped(){
    updateTimer.stop();
    updateStatistics(false);
}

void StatisticsWidget::onTimerReset(){
    clear();
}

void StatisticsWidget::onStatisticChanged(){
    updateStatistics(false);
}

void StatisticsWidget::onCubeSolved(){
    ui->timeLabel->setStyleSheet("font: bold; color: #008800");
}

void StatisticsWidget::onBackgroundColourSettingChanged(){
    //generate a paintEvent to repaint the widget with the new background colour
    update();
}
