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
    virtual QBrush brush() const override;
    virtual QPen pen() const override;
    void setBrush(const QBrush &p_brush) override;
    void setPen(const QPen &p_pen) override;
    void moveBy(const QPointF &p_delta_pos) override;
    void initizlizeShape(const QPointF &p_start, const QPointF &p_end) override;
    int fromSvgElement(const QDomElement &p_element, const QTransform &p_transform) override;
    virtual QRectF boundingRect() const override;
    virtual bool contains(const QPointF &p_point) const override;
    virtual void paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget = nullptr) override;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    qreal k_min_point_distance_ = 1.5;
    void setGrabbersPositions() override;
    int grabbersCount() override;
    bool pointToLineProjection(const QPointF &p_point, const QLineF &p_line, QPointF *p_projection_point) const;

private slots:
    void doOnGrabberMoved(VeGrabberDot *grabber, const QPointF &p_scene_pos, Qt::MouseButtons p_action) override;
    void doOnGrabberRelease(VeGrabberDot *grabber) override;


};

#endif // VESHAPEPOLYLINE_H
