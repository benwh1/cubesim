#include "colorsettingwidget.h"
#include "ui_colorsettingwidget.h"

ColorSettingWidget::ColorSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorSettingWidget)
{
    ui->setupUi(this);

    color = Qt::black;
    ui->colorLabel->setText("");
}

ColorSettingWidget::~ColorSettingWidget()
{
    delete ui;
}

QColor ColorSettingWidget::getColor(){
    return color;
}

void ColorSettingWidget::setColor(QColor c){
    color = c;

    ui->colorLabel->setText(color.name(QColor::HexArgb));
    ui->colorLabel->setStyleSheet("background-color: " + color.name(QColor::HexArgb));

    emit colorChanged();
}

void ColorSettingWidget::mousePressEvent(QMouseEvent *){
    QColor c = QColorDialog::getColor(color, this, "Select colour", QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);

    if(c.isValid()){
        setColor(c);
    }
}
