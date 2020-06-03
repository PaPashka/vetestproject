#ifndef VESHAPEITEM_H
#define VESHAPEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QAbstractGraphicsShapeItem>
#include <QBrush>
#include <QPen>
#include <QDomElement>
#include "vegrabberdot.h"

class VeShapeItem : public QObject
{
    Q_OBJECT
public:
    explicit VeShapeItem(QObject *parent = nullptr);

    enum {
        RECTANGLE   = QGraphicsItem::UserType + 1,
        GRABBER     = QGraphicsItem::UserType + 2,
        POLYLINE    = QGraphicsItem::UserType + 3
    };

    virtual int type() const = 0;
    virtual QBrush brush() const;
    virtual QPen pen() const;
    virtual void setBrush(const QBrush &) = 0;
    virtual void setPen(const QPen &) = 0;
    virtual void moveBy(const QPointF &) = 0;
    virtual void initizlizeShape(const QPointF &, const QPointF &) = 0;
    virtual int fromSvgElement(const QDomElement &, const QTransform &) = 0;


signals:

private slots:
    virtual void setSelected(const VeShapeItem *p_item);
    virtual void doOnGrabberMoved(VeGrabberDot *, const QPointF &, Qt::MouseButtons);
    virtual void doOnGrabberRelease(VeGrabberDot *);

protected:
    QList<VeGrabberDot *> grabbers_;
    virtual void addGrabber(int p_index = -1, bool p_state = false);

private:
    virtual void setGrabbersState(bool p_state);
    virtual void showGrabbers();
    virtual void hideGrabbers();
    virtual void setGrabbersPositions() = 0;


};

#endif // VESHAPEITEM_H
