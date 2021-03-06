#ifndef CUBEGRAPHICSVIEW_H
#define CUBEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QInputDialog>
#include <QStyleOptionGraphicsItem>
#include "cubegraphicsobject.h"
#include "settings.h"

class CubeGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CubeGraphicsView(QWidget *parent = nullptr);
    void initialize(Cube *cube, Settings *settings);

    void zoom(qreal factor);
    void resetZoom();

    void setCubeProjection(QString matrix);
    void setCubeProjection(float *matrix);
    void resetCubeProjection();

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QGraphicsScene *scene;

    Cube *cube;
    CubeGraphicsObject *cubeGraphicsObject;
    Settings *settings;

    qreal zoomFactor;

    void recalculateSceneRect();

private slots:
    void onProjectionChanged();

    //slots from settings being changed
    void onBackgroundColourSettingChanged();
    void onBorderPaddingSettingChanged();
    void onAntialiasingSettingChanged();

signals:
    void moveDrag(Axis axis, int layer, bool clockwise, Qt::MouseButton button);

};

#endif // CUBEGRAPHICSVIEW_H
