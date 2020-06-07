#ifndef COLORSETTINGWIDGET_H
#define COLORSETTINGWIDGET_H

#include <QColorDialog>
#include <QDebug>
#include <QWidget>

namespace Ui {
class ColorSettingWidget;
}

class ColorSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorSettingWidget(QWidget *parent = nullptr);
    ~ColorSettingWidget();

    QColor getColor();

    void setColor(QColor c);

protected:
    void mousePressEvent(QMouseEvent *);

private:
    Ui::ColorSettingWidget *ui;

    QColor color;

signals:
    void colorChanged();

};

#endif // COLORSETTINGWIDGET_H
