#include "vegrabberdot.h"

VeGrabberDot::VeGrabberDot(QObject *parent, QGraphicsItem *parent_item)
    : QObject(parent)
{
    setParentItem(parent_item);
    setAcceptHoverEvents(true);
    setBrush(QBrush(Qt::white));
    setPen(QPen(Qt::black, 1));
    hide();
    setRect(-3, -3, 6, 6);
}

int VeGrabberDot::type() const
{
    return Type;
}

void VeGrabberDot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    start_delta_pos_ = event->pos() - rect().center();
}

void VeGrabberDot::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit itemMoved(this, event->scenePos() - start_delta_pos_, event->buttons());
}

void VeGrabberDot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    emit itemRelease(this);
}

void VeGrabberDot::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    setBrush(QBrush(Qt::red));
    setPen(QPen(Qt::red, 1));
}

void VeGrabberDot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    setBrush(QBrush(Qt::white));
    setPen(QPen(Qt::black, 1));
}
