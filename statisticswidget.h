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

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::StatisticsWidget *ui;

    bool initialized;

    Statistics *statistics;
    Settings *settings;

    QTimer updateTimer;

    void updateStatistics();
    void clear();

private slots:
    void onTimeout();
    void onTimerStarted();
    void onTimerStopped();
    void onTimerReset();
    void onMoveDone();

    //slots from settings being changed
    void onBackgroundColourSettingChanged();

};

#endif // STATISTICSWIDGET_H
