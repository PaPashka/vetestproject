#include "veshapeitem.h"

VeShapeItem::VeShapeItem(QObject *parent)
    : QObject(parent)
{
    connect(parent, SIGNAL(itemSelected(const VeShapeItem *)), this, SLOT(setSelected(const VeShapeItem *)));
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

void VeShapeItem::setSelected(const VeShapeItem *p_item)
{
    setGrabbersState(this == p_item);
}

void VeShapeItem::doOnGrabberMoved(VeGrabberDot *, const QPointF &, Qt::MouseButtons)
{

}

void VeShapeItem::doOnGrabberRelease(VeGrabberDot *)
{

}

void VeShapeItem::setGrabbersState(bool p_state)
{
    if (p_state) {
        showGrabbers();
    }
    else {
        hideGrabbers();
    }
}

void VeShapeItem::showGrabbers()
{
    for (int i = 0; i < grabbers_.count(); i++) {
        grabbers_[i]->show();
    }
}

void VeShapeItem::hideGrabbers()
{
    for (int i = 0; i < grabbers_.count(); i++) {
        grabbers_[i]->hide();
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

