#ifndef VESHAPERECT_H
#define VESHAPERECT_H

#include <QObject>
#include <QGraphicsRectItem>
#include "veshapeitem.h"

class VeShapeRect : public VeShapeItem, public QGraphicsRectItem
{
    Q_OBJECT

public:
    explicit VeShapeRect(QObject *parent = nullptr);

    enum { Type = RECTANGLE };

    enum GrabberType {
        GRABBER_TOP_LEFT,
        GRABBER_TOP_RIGHT,
        GRABBER_BOTTOM_LEFT,
        GRABBER_BOTTOM_RIGHT,
        GRABBER_TOP,
        GRABBER_RIGHT,
        GRABBER_BOTTOM,
        GRABBER_LEFT
    };

    int type() const override;
    virtual QBrush brush() const override;
    virtual QPen pen() const override;
    void setBrush(const QBrush &p_brush) override;
    void setPen(const QPen &p_pen) override;
    void moveBy(const QPointF &p_delta_pos) override;
    QPainterPath shape() const override;
    void initizlizeShape(const QPointF &p_start, const QPointF &p_end) override;
    int fromSvgElement(const QDomElement &p_element, const QTransform &p_transform) override;
    virtual QRectF boundingRect() const override;
    virtual bool contains(const QPointF &p_point) const override;
    virtual void paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget = nullptr) override;

private:
    void setRect(const QRectF &p_rectangle);
    void setRect(qreal p_x, qreal p_y, qreal p_width, qreal p_height);
    void setGrabbersPositions() override;
    void rotate(qreal angle);
    int grabbersCount() override;

private slots:
    void doOnGrabberMoved(VeGrabberDot *grabber, const QPointF &p_pos, Qt::MouseButtons p_action) override;

};

#endif // VESHAPERECT_H
