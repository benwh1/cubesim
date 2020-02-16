#ifndef CUBEGRAPHICSVIEW_H
#define CUBEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QInputDialog>
#include "cubegraphicsobject.h"
#include "settings.h"

class CubeGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CubeGraphicsView(QWidget *parent = nullptr);
    void initialize(CubeState *cube, Settings *settings);

    void zoom(qreal factor);

    void setCubeProjection(QString matrix);
    void setCubeProjection(float *matrix);
    void resetCubeProjection();

    CubeGraphicsObject *getCubeGraphicsObject();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QGraphicsScene *scene;

    CubeState *cube;
    CubeGraphicsObject *cubeGraphicsObject;
    Settings *settings;

    qreal zoomFactor;

private slots:
    void onProjectionChanged();

    //slots from settings being changed
    void onBackgroundColourSettingChanged();
    void onAntialiasingSettingChanged();

signals:
    void moveDrag(Axis axis, int layer, bool clockwise, Qt::MouseButton button);

};

#endif // CUBEGRAPHICSVIEW_H
