#ifndef REPLAYRECORDERSETTINGSWIDGET_H
#define REPLAYRECORDERSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class ReplayRecorderSettingsWidget;
}

class ReplayRecorderSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReplayRecorderSettingsWidget(QWidget *parent = nullptr);
    ~ReplayRecorderSettingsWidget();

private:
    Ui::ReplayRecorderSettingsWidget *ui;
};

#endif // REPLAYRECORDERSETTINGSWIDGET_H
