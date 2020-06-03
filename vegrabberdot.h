#ifndef VEGRABBERDOT_H
#define VEGRABBERDOT_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>

class VeGrabberDot : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit VeGrabberDot(QObject *parent_object = nullptr, QGraphicsItem *parent_item = nullptr);

    enum { Type = 65536 + 2 };

    int type() const override;

signals:
    void itemMoved(VeGrabberDot *grabber, const QPointF &p_pos, Qt::MouseButtons p_action);
    void itemRelease(VeGrabberDot *grabber);

private:
    QPointF start_delta_pos_;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

};

#endif // VEGRABBERDOT_H
