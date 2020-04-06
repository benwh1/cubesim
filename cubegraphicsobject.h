#ifndef CUBEGRAPHICSOBJECT_H
#define CUBEGRAPHICSOBJECT_H

#include <QApplication>
#include <QBrush>
#include <QDebug>
#include <QElapsedTimer>
#include <QGraphicsObject>
#include <QGraphicsSceneDragDropEvent>
#include <QKeyEvent>
#include <QPen>
#include <QVector3D>
#include <cmath>
#include "cube.h"
#include "enums.h"
#include "projection.h"
#include "settings.h"
#include "sticker.h"

#define pi (4*atan(1))

class CubeGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit CubeGraphicsObject(Cube *c = nullptr, Settings *s = nullptr, QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

    void setEdgeLength(qreal edgeLength);
    void setGapSize(qreal gapSize);

    void setProjection(float *mat);

    void updateAll();

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

//private:
public:
    Cube *cube;
    Settings *settings;

    qreal edgeLength;
    qreal gapSize;

    QList<QList<QList<Sticker*>>> stickers;
    QList<QGraphicsLineItem*> guideLinesCross;
    QList<QGraphicsLineItem*> guideLinesPlus;
    QList<QGraphicsPolygonItem*> guideLinesBox;

    Projection proj;

    QPointF lastMousePress;
    QPointF lastMouseRelease;

    void reset();
    void updateSticker(Face face, int x, int y);
    void updateFace(Face face);
    void updateLayer(Axis axis, int layer);

private slots:
    void onMoveDone(Move move);
    void onRotationDone();
    void onCubeReset();
    void onCubeScrambled();
    void onCubeStateChanged();
    void onCubeSizeChanged();

    //slots from settings being changed
    void onLineColourSettingChanged();
    void onLineWidthSettingChanged();
    void onColoursSettingChanged();
    void onGuideLinesCrossSettingChanged();
    void onGuideLinesPlusSettingChanged();
    void onGuideLinesBoxSettingChanged();
    void onGuideLineColourSettingChanged();
    void onGuideLineWidthSettingChanged();
    void onSupercubeSettingChanged();
    void onSupercubeStickersSettingChanged();
    void onPochmannBarThicknessSettingChanged();
    void onPochmannCageSettingChanged();

signals:
    void projectionChanged();
    void moveDrag(Axis axis, int layer, bool clockwise, Qt::MouseButton button);

};

#endif // CUBEGRAPHICSOBJECT_H
