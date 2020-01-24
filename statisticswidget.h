#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QDebug>
#include <QTimer>
#include <QWidget>
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

    void initialize(Statistics *statistics);

private:
    Ui::StatisticsWidget *ui;

    bool initialized;

    Statistics *statistics;
    QTimer updateTimer;

    void updateStatistics();
    void clear();

private slots:
    void onTimeout();
    void onTimerStarted();
    void onTimerStopped();
    void onTimerReset();
    void onMoveDone();

};

#endif // STATISTICSWIDGET_H
