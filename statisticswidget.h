#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QWidget>
#include "settings.h"
#include "statistics.h"

namespace Ui {
class StatisticsWidget;
}

class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(QWidget *parent = nullptr);
    ~StatisticsWidget();

    void initialize(Statistics *statistics, Settings *settings);

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::StatisticsWidget *ui;

    bool initialized;

    Statistics *statistics;
    Settings *settings;

    QTimer updateTimer;

    //if force is true, then the statistics will be updated even if the
    //timer in statistics is not running. otherwise, nothing will be updated
    void updateStatistics(bool force = false);
    void clear();

private slots:
    void onTimeout();
    void onTimerStarted();
    void onTimerStopped();
    void onTimerReset();
    void onMoveDone();

    void onCubeSolved();

    //slots from settings being changed
    void onBackgroundColourSettingChanged();

};

#endif // STATISTICSWIDGET_H
