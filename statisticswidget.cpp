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
    connect(statistics, SIGNAL(moveDone()), this, SLOT(onMoveDone()));
    connect(statistics, SIGNAL(timerReset()), this, SLOT(onTimerReset()));

    this->settings = settings;

    //connect to signals that are emitted from settings being changed
    connect(settings, SIGNAL(backgroundColourChanged()), this, SLOT(onBackgroundColourSettingChanged()));

    initialized = true;
}

void StatisticsWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setBrush(QBrush(settings->getBackgroundColour()));
    painter.drawRect(0, 0, width()-1, height()-1);
    QWidget::paintEvent(event);
}

void StatisticsWidget::updateStatistics(){
    //make sure we actually have a statistics object to work with
    if(!initialized) return;

    if(statistics->timerRunning()){
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
    updateStatistics();
}

void StatisticsWidget::onTimerStarted(){
    ui->timeLabel->setStyleSheet("");

    updateTimer.start(30);
    updateStatistics();
}

void StatisticsWidget::onTimerStopped(){
    updateTimer.stop();
    updateStatistics();
}

void StatisticsWidget::onTimerReset(){
    clear();
}

void StatisticsWidget::onMoveDone(){
    updateStatistics();
}

void StatisticsWidget::onCubeSolved(){
    ui->timeLabel->setStyleSheet("font: bold; color: #008800");
}

void StatisticsWidget::onBackgroundColourSettingChanged(){
    //generate a paintEvent to repaint the widget with the new background colour
    update();
}
