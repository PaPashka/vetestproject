#include "veshapeitem.h"

VeShapeItem::VeShapeItem(QObject *parent)
    : QObject(parent)
    , applied_pattern_(NO_PATTERN)
{
    connect(parent, SIGNAL(itemSelected(const VeShapeItem *)), this, SLOT(itemSelectionEvent(const VeShapeItem *)));
    connect(parent, SIGNAL(itemUnderCursorChanged(const VeShapeItem *)), this, SLOT(itemUnderCursorEvent(const VeShapeItem *)));
}

void VeShapeItem::itemSelectionEvent(const VeShapeItem *p_item)
{
    for (int i = 0; i < grabbers_.count(); i++) {
        grabbers_[i]->setVisible(this == p_item);
    }
}

void VeShapeItem::itemUnderCursorEvent(const VeShapeItem *p_item)
{
    applied_pattern_ = (this == p_item)? UNDER_CURSOR : NO_PATTERN;
}

void VeShapeItem::addGrabber(int p_index, bool p_state)
{
        VeGrabberDot *new_grabber = new VeGrabberDot(this, dynamic_cast<QGraphicsItem *>(this));

        connect(new_grabber, SIGNAL(itemMoved(VeGrabberDot *, const QPointF &, Qt::MouseButtons)),
                this, SLOT(doOnGrabberMoved(VeGrabberDot *, const QPointF &, Qt::MouseButtons)));
        connect(new_grabber, SIGNAL(itemRelease(VeGrabberDot *)),
                this, SLOT(doOnGrabberRelease(VeGrabberDot *)));
        connect(parent(), SIGNAL(itemUnderCursorChanged(const VeShapeItem *)),
                new_grabber, SLOT(itemUnderCursorEvent(const VeShapeItem *)));


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
    QRectF item_bounding_rect = boundingRect();

    switch (applied_pattern_) {
        case UNDER_CURSOR: {
            int pattern_delta_y = 3;
            int pattern_delta_x = 5;
            int dot_delta_y = 0;

            while (dot_delta_y <= item_bounding_rect.height()) {
                p_painter->setPen((((dot_delta_y / pattern_delta_y) % 2) == 0)? Qt::darkGray : Qt::lightGray);
                int dot_delta_x = pattern_delta_x - (dot_delta_y % pattern_delta_x);
                while (dot_delta_x <= item_bounding_rect.width()) {

                    if (contains(QPointF(item_bounding_rect.x() + dot_delta_x,
                                         item_bounding_rect.y() + dot_delta_y))) {
                        p_painter->drawPoint(item_bounding_rect.x() + dot_delta_x,
                                             item_bounding_rect.y() + dot_delta_y);
                    }
                    dot_delta_x += pattern_delta_x;
                }
                dot_delta_y += pattern_delta_y;
            }
            break;
        }
        default: {
            break;
        }
    }

}

