#ifndef VESHAPEPOLYLINE_H
#define VESHAPEPOLYLINE_H

#include <QObject>
#include <QPainterPath>
#include "veshapeitem.h"

class VeShapePolyline : public VeShapeItem, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VeShapePolyline(QObject *parent_object = nullptr);

    enum { Type = POLYLINE };

    int type() const override;
    void setBrush(const QBrush &p_brush) override;
    void setPen(const QPen &p_pen) override;
    void moveBy(const QPointF &p_delta_pos) override;
    void initizlizeShape(const QPointF &p_start, const QPointF &p_end) override;
    int fromSvgElement(const QDomElement &p_element, const QTransform &p_transform) override;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void setGrabbersPositions() override;
    void initializeGrabbers();

private slots:
    void doOnGrabberMoved(VeGrabberDot *grabber, const QPointF &p_scene_pos, Qt::MouseButtons p_action) override;
    void doOnGrabberRelease(VeGrabberDot *grabber) override;

};

#endif // VESHAPEPOLYLINE_H
