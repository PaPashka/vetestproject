#include "vegrabberdot.h"

VeGrabberDot::VeGrabberDot(QObject *parent, QGraphicsItem *parent_item)
    : QObject(parent)
    , is_active_(false)
    , is_action_in_progress_(false)
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

void VeGrabberDot::itemUnderCursorEvent(const VeShapeItem *p_item)
{
    is_active_ = p_item == nullptr;
}

void VeGrabberDot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (is_active_) {
        start_delta_pos_ = event->pos() - rect().center();
        is_action_in_progress_ = true;
    }
}

void VeGrabberDot::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (is_action_in_progress_) {
        emit itemMoved(this, event->scenePos() - start_delta_pos_, event->buttons());
    }
}

void VeGrabberDot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    emit itemRelease(this);
    is_action_in_progress_ = false;
}

void VeGrabberDot::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    if (is_active_) {
        setBrush(QBrush(Qt::red));
        setPen(QPen(Qt::red, 1));
    }
}

void VeGrabberDot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    setBrush(QBrush(Qt::white));
    setPen(QPen(Qt::black, 1));
}
