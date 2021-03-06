#ifndef VEGRABBERDOT_H
#define VEGRABBERDOT_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsView>

class VeShapeItem;

class VeGrabberDot : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit VeGrabberDot(QObject *parent_object = nullptr, QGraphicsItem *parent_item = nullptr,
                          bool p_visible = false, bool p_active = false, bool p_highlight = false);

    enum { Type = 65536 + 2 };

    int type() const override;
    void highlight(bool p_state);

signals:
    void itemMoved(VeGrabberDot *grabber, const QPointF &p_pos, Qt::MouseButtons p_action);
    void itemRelease(VeGrabberDot *grabber);

private slots:
    void itemUnderCursorEvent(const QGraphicsItem *p_item);

private:
    bool is_active_;
    QPointF start_delta_pos_;
    bool is_action_in_progress_;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

};

#endif // VEGRABBERDOT_H
