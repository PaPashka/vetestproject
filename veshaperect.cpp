#include "veshaperect.h"

VeShapeRect::VeShapeRect(QObject *parent)
    : VeShapeItem(parent)
{
    initializeGrabbers();
}

int VeShapeRect::type() const
{
    return Type;
}

QBrush VeShapeRect::brush() const
{
    return QGraphicsRectItem::brush();
}

QPen VeShapeRect::pen() const
{
    return QGraphicsRectItem::pen();
}

void VeShapeRect::setBrush(const QBrush &p_brush)
{
    QGraphicsRectItem::setBrush(p_brush);
}

void VeShapeRect::setPen(const QPen &p_pen)
{
    QGraphicsRectItem::setPen(p_pen);
}

void VeShapeRect::moveBy(const QPointF &p_pos)
{
    QPointF local_delta_pos = mapFromScene(p_pos) - mapFromScene(QPointF(0, 0));
    setRect(rect().x() + local_delta_pos.x(), rect().y() + local_delta_pos.y(), rect().width(), rect().height());
}

QPainterPath VeShapeRect::shape() const
{
    return QGraphicsRectItem::shape();
}

void VeShapeRect::initizlizeShape(const QPointF &p_start, const QPointF &p_end)
{
    resetTransform();

    QPointF local_start = mapFromScene(p_start);
    QPointF local_end = mapFromScene(p_end);

    qreal new_x = qMin(local_start.x(), local_end.x());
    qreal new_y = qMin(local_start.y(), local_end.y());
    qreal new_w = (local_start.x() == local_end.x())? 1 : qAbs(local_start.x() - local_end.x());
    qreal new_h = (local_start.y() == local_end.y())? 1 : qAbs(local_start.y() - local_end.y());

    setRect(new_x, new_y, new_w, new_h);
}

int VeShapeRect::fromSvgElement(const QDomElement &p_element, const QTransform &p_transform)
{
    if (p_element.hasAttribute("x") && p_element.hasAttribute("y") &&
        p_element.hasAttribute("width") && p_element.hasAttribute("height")) {

        resetTransform();

        setRect(p_element.attribute("x", "0").toFloat(), p_element.attribute("y", "0").toFloat(),
                p_element.attribute("width", "0").toFloat(), p_element.attribute("height", "0").toFloat());

        setTransform(p_transform);

        initializeGrabbers();
        return 0;

    } else {
        return -1;
    }
}

QRectF VeShapeRect::boundingRect() const
{
    return QGraphicsRectItem::boundingRect();
}

bool VeShapeRect::contains(const QPointF &p_point) const
{
    if (brush().color().alpha() > 0) {
        return QGraphicsRectItem::contains(p_point);
    } else {
        qreal offset_k = pen().width();
        QRectF item_bounding_rect = boundingRect();
        if (item_bounding_rect.x() + offset_k < item_bounding_rect.right() &&
            item_bounding_rect.y() + offset_k < item_bounding_rect.bottom() &&
            item_bounding_rect.width() - 2 * offset_k > 0 &&
            item_bounding_rect.height() - 2 * offset_k > 0) {
            QRectF item_inside_rect(item_bounding_rect.x() + offset_k, item_bounding_rect.y() + offset_k,
                                    item_bounding_rect.width() - 2 * offset_k, item_bounding_rect.height() - 2 * offset_k);
            return QGraphicsRectItem::contains(p_point) && !item_inside_rect.contains(p_point);
        } else {
            return QGraphicsRectItem::contains(p_point);
        }
    }
}

void VeShapeRect::paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget)
{
    QGraphicsRectItem::paint(p_painter, p_option, p_widget);

    drawPattern(p_painter);
}

void VeShapeRect::setRect(const QRectF &p_rectangle)
{
    setRect(p_rectangle.x(), p_rectangle.y(), p_rectangle.width(), p_rectangle.height());
}

void VeShapeRect::setRect(qreal p_x, qreal p_y, qreal p_width, qreal p_height)
{
    QGraphicsRectItem::setRect(p_x, p_y, p_width, p_height);
    setGrabbersPositions();
}

void VeShapeRect::rotate(qreal angle)
{
    QRectF item_rect = rect();
    QTransform transform_item = transform();

    transform_item.translate(item_rect.center().x(), item_rect.center().y());
    transform_item.rotate(angle);
    transform_item.translate(- item_rect.center().x(), - item_rect.center().y());

    setTransform(transform_item);
}

void VeShapeRect::setGrabbersPositions()
{
    QRectF item_rect = rect();

    grabbers_[GRABBER_TOP_LEFT]->setPos(item_rect.topLeft());
    grabbers_[GRABBER_TOP_RIGHT]->setPos(item_rect.topRight());
    grabbers_[GRABBER_BOTTOM_LEFT]->setPos(item_rect.bottomLeft());
    grabbers_[GRABBER_BOTTOM_RIGHT]->setPos(item_rect.bottomRight());
    grabbers_[GRABBER_TOP]->setPos(item_rect.left() + item_rect.width()/2, item_rect.top());
    grabbers_[GRABBER_BOTTOM]->setPos(item_rect.left() + item_rect.width()/2, item_rect.bottom());
    grabbers_[GRABBER_LEFT]->setPos(item_rect.left(), item_rect.top() + item_rect.height()/2);
    grabbers_[GRABBER_RIGHT]->setPos(item_rect.right(), item_rect.top() + item_rect.height()/2);

}

void VeShapeRect::doOnGrabberMoved(VeGrabberDot *grabber, const QPointF &p_scene_pos, Qt::MouseButtons p_action)
{
    QRectF item_rect = rect();
    QPointF local_pos = mapFromScene(p_scene_pos);

    int grabber_type = grabbers_.indexOf(grabber);

    if (p_action & Qt::LeftButton) {

        switch(grabber_type) {
            case GRABBER_TOP_LEFT: {
                item_rect.setLeft((local_pos.x() < item_rect.right())? local_pos.x() : item_rect.right() - 1);
                item_rect.setTop((local_pos.y() < item_rect.bottom())? local_pos.y() : item_rect.bottom() - 1);
                break;
            }
            case GRABBER_TOP_RIGHT: {
                item_rect.setRight((local_pos.x() > item_rect.left())? local_pos.x() : item_rect.left() + 1);
                item_rect.setTop((local_pos.y() < item_rect.bottom())? local_pos.y() : item_rect.bottom() - 1);
                break;
            }
            case GRABBER_BOTTOM_LEFT: {
                item_rect.setLeft((local_pos.x() < item_rect.right())? local_pos.x() : item_rect.right() - 1);
                item_rect.setBottom((local_pos.y() > item_rect.top())? local_pos.y() : item_rect.top() + 1);
                break;
            }
            case GRABBER_BOTTOM_RIGHT: {
                item_rect.setRight((local_pos.x() > item_rect.left())? local_pos.x() : item_rect.left() + 1);
                item_rect.setBottom((local_pos.y() > item_rect.top())? local_pos.y() : item_rect.top() + 1);
                break;
            }
            case GRABBER_TOP: {
                item_rect.setTop((local_pos.y() < item_rect.bottom())? local_pos.y() : item_rect.bottom() - 1);
                break;
            }
            case GRABBER_RIGHT: {
                item_rect.setRight((local_pos.x() > item_rect.left())? local_pos.x() : item_rect.left() + 1);
                break;
            }
            case GRABBER_BOTTOM: {
                item_rect.setBottom((local_pos.y() > item_rect.top())? local_pos.y() : item_rect.top() + 1);
                break;
            }
            case GRABBER_LEFT: {
                item_rect.setLeft((local_pos.x() < item_rect.right())? local_pos.x() : item_rect.right() - 1);
                break;
            }
            default: {
                break;
            }
        }

        setRect(item_rect);
        setGrabbersPositions();

    } else if (p_action & Qt::RightButton) {

        QLineF line_center_to_cursor(item_rect.center(), local_pos);
        QLineF line_center_to_corner(item_rect.center(), grabber->pos());
        qreal rotation_angle = line_center_to_cursor.angleTo(line_center_to_corner);

        rotate(rotation_angle);

    }
}

int VeShapeRect::grabbersCount()
{
    return 8;
}
