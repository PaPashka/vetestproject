#include "vegrabberdot.h"

VeGrabberDot::VeGrabberDot(QObject *parent, QGraphicsItem *parent_item,
                           bool p_visible, bool p_active, bool p_highlight)
    : QObject(parent)
    , is_active_(p_active)
    , is_action_in_progress_(false)
{
    setParentItem(parent_item);
    setAcceptHoverEvents(true);
    setBrush(QBrush(Qt::white));
    setPen(QPen(Qt::black, 1));
    if (p_visible) {
        show();
    } else {
        hide();
    }
    if (p_visible && p_highlight && p_active) {
        highlight(true);
    }
    setRect(-3, -3, 6, 6);
}

int VeGrabberDot::type() const
{
    return Type;
}

void VeGrabberDot::highlight(bool p_state)
{
    if (p_state) {
        setBrush(QBrush(Qt::red));
        setPen(QPen(Qt::red, 1));
    } else {
        setBrush(QBrush(Qt::white));
        setPen(QPen(Qt::black, 1));
    }
}

void VeGrabberDot::itemUnderCursorEvent(const QGraphicsItem *p_item)
{
    is_active_ = dynamic_cast<QGraphicsItem *>(this) == p_item;
    if (is_active_) {
        highlight(true);
    }
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

void VeGrabberDot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    highlight(false);
}
