#include "veshapeitem.h"

VeShapeItem::VeShapeItem(QObject *parent)
    : QObject(parent)
{
    connect(parent, SIGNAL(itemSelected(const VeShapeItem *)), this, SLOT(itemSelectionEvent(const VeShapeItem *)));
}

QBrush VeShapeItem::brush() const
{
    if (dynamic_cast<const QAbstractGraphicsShapeItem *>(this)) {
        return dynamic_cast<const QAbstractGraphicsShapeItem *>(this)->brush();
    }
    return QBrush();
}

QPen VeShapeItem::pen() const
{
    if (dynamic_cast<const QAbstractGraphicsShapeItem *>(this)) {
        return dynamic_cast<const QAbstractGraphicsShapeItem *>(this)->pen();
    }
    return QPen();
}

void VeShapeItem::itemSelectionEvent(const VeShapeItem *p_item)
{
    for (int i = 0; i < grabbers_.count(); i++) {
        grabbers_[i]->setVisible(this == p_item);
    }
}

void VeShapeItem::addGrabber(int p_index, bool p_state)
{
        VeGrabberDot *new_grabber = new VeGrabberDot(this, dynamic_cast<QGraphicsItem *>(this));

        connect(new_grabber, SIGNAL(itemMoved(VeGrabberDot *, const QPointF &, Qt::MouseButtons)),
                this, SLOT(doOnGrabberMoved(VeGrabberDot *, const QPointF &, Qt::MouseButtons)));
        connect(new_grabber, SIGNAL(itemRelease(VeGrabberDot *)),
                this, SLOT(doOnGrabberRelease(VeGrabberDot *)));

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

