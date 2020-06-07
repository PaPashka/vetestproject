#include "veshapeitem.h"

VeShapeItem::VeShapeItem(QObject *parent)
    : QObject(parent)
    , applied_pattern_(NO_PATTERN)
{
    connect(parent, SIGNAL(itemSelected(const VeShapeItem *)), this, SLOT(itemSelectionEvent(const VeShapeItem *)));
    connect(parent, SIGNAL(itemUnderCursorChanged(const QGraphicsItem *)), this, SLOT(itemUnderCursorEvent(const QGraphicsItem *)));
}

void VeShapeItem::itemSelectionEvent(const VeShapeItem *p_item)
{
    for (int i = 0; i < grabbers_.count(); i++) {
        grabbers_[i]->setVisible(this == p_item);
    }
}

void VeShapeItem::itemUnderCursorEvent(const QGraphicsItem *p_item)
{
    applied_pattern_ = (dynamic_cast<QGraphicsItem *>(this) == p_item)? UNDER_CURSOR : NO_PATTERN;
}

void VeShapeItem::addGrabber(int p_index, bool p_state)
{
        VeGrabberDot *new_grabber = new VeGrabberDot(this, dynamic_cast<QGraphicsItem *>(this));

        connect(new_grabber, SIGNAL(itemMoved(VeGrabberDot *, const QPointF &, Qt::MouseButtons)),
                this, SLOT(doOnGrabberMoved(VeGrabberDot *, const QPointF &, Qt::MouseButtons)));
        connect(new_grabber, SIGNAL(itemRelease(VeGrabberDot *)),
                this, SLOT(doOnGrabberRelease(VeGrabberDot *)));
        connect(parent(), SIGNAL(itemUnderCursorChanged(const QGraphicsItem *)),
                new_grabber, SLOT(itemUnderCursorEvent(const QGraphicsItem *)));


        if (p_index < 0 || p_index >= grabbers_.count() ) {
            grabbers_.append(new_grabber);
        } else {
            grabbers_.insert(p_index, new_grabber);
        }

        if (p_state) {
            new_grabber->show();
        } else {
            new_grabber->hide();
        }
}

void VeShapeItem::initializeGrabbers()
{
    int delta_count_grabbers = grabbersCount() - grabbers_.count();

    if (delta_count_grabbers < 0) {
        for(int i = 0; i < delta_count_grabbers ; i++) {
            delete grabbers_.last();
            grabbers_.removeLast();
        }
    } else if (delta_count_grabbers > 0) {
        for (int i = 0; i < delta_count_grabbers; i++) {
            addGrabber();
        }
    }
    setGrabbersPositions();
}

void VeShapeItem::drawPattern(QPainter *p_painter)
{
    switch (applied_pattern_) {
        case UNDER_CURSOR: {
            QPen pattern_pen(Qt::red);
            QColor pattern_brush_color(Qt::red);
            pattern_brush_color.setAlpha((brush().color().alpha() > 0)? 255: 0);
            QBrush pattern_brush(pattern_brush_color);
            pattern_brush.setStyle(Qt::Dense6Pattern);
            p_painter->setPen(pattern_pen);
            p_painter->setBrush(pattern_brush);
            p_painter->drawPath(shape());

            break;
        }
        default: {
            break;
        }
    }

}

