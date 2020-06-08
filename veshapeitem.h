#ifndef VESHAPEITEM_H
#define VESHAPEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QAbstractGraphicsShapeItem>
#include <QBrush>
#include <QPen>
#include <QDomElement>
#include <QPainter>
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

    enum {
        NO_PATTERN,
        UNDER_CURSOR
    };

    virtual int type() const = 0;
    virtual QBrush brush() const = 0;
    virtual QPen pen() const = 0;
    virtual void setBrush(const QBrush &) = 0;
    virtual void setPen(const QPen &) = 0;
    virtual void moveBy(const QPointF &) = 0;
    virtual void initizlizeShape(const QPointF &, const QPointF &) = 0;
    virtual int fromSvgElement(const QDomElement &, const QTransform &) = 0;
    virtual QRectF boundingRect() const = 0;
    virtual bool contains(const QPointF &) const = 0;
    virtual QPainterPath shape() const = 0;

signals:

private slots:
    virtual void itemSelectionEvent(const VeShapeItem *p_item);
    virtual void doOnGrabberMoved(VeGrabberDot *, const QPointF &, Qt::MouseButtons) {};
    virtual void doOnGrabberRelease(VeGrabberDot *) {};
    virtual void itemUnderCursorEvent(const QGraphicsItem *p_item);

protected:
    QList<VeGrabberDot *> grabbers_;
    virtual void addGrabber(int p_index = -1, bool p_visible = false, bool p_active = false, bool p_highlight = false);
    virtual void initializeGrabbers();
    void drawPattern(QPainter *painter);

private:
    int applied_pattern_;
    virtual void setGrabbersPositions() = 0;
    virtual int grabbersCount() = 0;

};

#endif // VESHAPEITEM_H
