#include "veshapepolyline.h"

VeShapePolyline::VeShapePolyline(QObject *parent)
    : VeShapeItem(parent)
{

}

int VeShapePolyline::type() const
{
    return Type;
}

QBrush VeShapePolyline::brush() const
{
    return QGraphicsPathItem::brush();
}

QPen VeShapePolyline::pen() const
{
    return QGraphicsPathItem::pen();
}

void VeShapePolyline::setBrush(const QBrush &p_brush)
{
    QGraphicsPathItem::setBrush(p_brush);
}

void VeShapePolyline::setPen(const QPen &p_pen)
{
    QGraphicsPathItem::setPen(p_pen);
}

void VeShapePolyline::moveBy(const QPointF &p_pos)
{
    QPointF local_delta_pos = mapFromScene(p_pos) - mapFromScene(QPointF(0, 0));
    QPainterPath new_path = path();

    for(int i = 0; i < new_path.elementCount(); i++) {
        new_path.setElementPositionAt(i, new_path.elementAt(i).x + local_delta_pos.x(),
                                         new_path.elementAt(i).y + local_delta_pos.y());
    }
    setPath(new_path);

    setGrabbersPositions();
}

void VeShapePolyline::initizlizeShape(const QPointF &p_start, const QPointF &p_end)
{
    resetTransform();

    QPointF local_start = mapFromScene(p_start);
    QPointF local_end = mapFromScene(p_end);

    QPainterPath new_path;
    QLineF check_line(local_start, local_end);
    if(check_line.length() == 0) {
        local_end.setX(local_end.x() + 1);
    }

    new_path.moveTo(local_start);
    new_path.lineTo(local_end);
    setPath(new_path);

    initializeGrabbers();
}

int VeShapePolyline::fromSvgElement(const QDomElement &p_element, const QTransform &p_transform)
{
    resetTransform();

    QPainterPath new_path;
    QStringList list_of_dotes = p_element.attribute("d").split(" ");

    for(int i = 0; i < list_of_dotes.length(); i++){
        QStringList dot_xy_list = list_of_dotes[i].right(list_of_dotes[i].length()-1).split(",");
        if (dot_xy_list.count() != 2) {
            return -1;
        }
        if (i == 0) {
            new_path.moveTo(dot_xy_list[0].toFloat(), dot_xy_list[1].toFloat());
        } else {
            new_path.lineTo(dot_xy_list[0].toFloat(), dot_xy_list[1].toFloat());
        }
    }

    setPath(new_path);
    setTransform(p_transform);
    initializeGrabbers();
    return 0;
}

QRectF VeShapePolyline::boundingRect() const
{
    return QGraphicsPathItem::boundingRect();
}

bool VeShapePolyline::contains(const QPointF &p_point) const
{
    QPainterPath line_path = path();
    qreal offset_k = pen().width() * 0.5;
    bool result = false;

    for(int i = 1; i < line_path.elementCount(); i++) {
        QLineF polyline_element(line_path.elementAt(i - 1), line_path.elementAt(i));
        QPointF intersects_point;

        result = result || (pointToLineProjection(p_point, polyline_element, &intersects_point)
                            && QLineF(intersects_point, p_point).length() <= offset_k);
        result = result || QLineF(p_point, polyline_element.p1()).length() <= offset_k;
        result = result || QLineF(p_point, polyline_element.p2()).length() <= offset_k;
    }
    if (brush().color().alpha() != 0) {
        result = result || QGraphicsPathItem::contains(p_point);
    }
    return result;
}

void VeShapePolyline::paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget)
{
    QGraphicsPathItem::paint(p_painter, p_option, p_widget);

    drawPattern(p_painter);
}

void VeShapePolyline::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::LeftButton && grabbers_[0]->isVisible()) {
        QPointF local_pos = mapFromScene(event->scenePos());
        QPainterPath old_path = path();
        QPainterPath new_path;
        qreal offset_k = pen().width() * 0.5;

        bool new_point_added = false;

        new_path.moveTo(old_path.elementAt(0));
        for(int i = 1; i < old_path.elementCount(); i++) {
            if (new_point_added) {
                new_path.lineTo(old_path.elementAt(i));
            } else {
                QLineF polyline_element(old_path.elementAt(i - 1), old_path.elementAt(i));
                QPointF new_path_point;

                if (pointToLineProjection(local_pos, polyline_element, &new_path_point)
                        && QLineF(new_path_point, local_pos).length() <= offset_k
                        && QLineF(new_path_point, polyline_element.p1()).length() >= k_min_point_distance_
                        && QLineF(new_path_point, polyline_element.p2()).length() >= k_min_point_distance_) {

                    new_path.lineTo(new_path_point);
                    new_path.lineTo(old_path.elementAt(i));

                    addGrabber(i, true);

                    new_point_added = true;
                } else {
                    new_path.lineTo(old_path.elementAt(i));
                }
            }
        }

        setPath(new_path);
        setGrabbersPositions();
    }
}

void VeShapePolyline::setGrabbersPositions()
{
    QPainterPath item_path = path();
    for(int i = 0; i < grabbers_.count(); i++) {
        grabbers_[i]->setPos(item_path.elementAt(i).x, item_path.elementAt(i).y);
    }
}

void VeShapePolyline::doOnGrabberMoved(VeGrabberDot *grabber, const QPointF &p_scene_pos, Qt::MouseButtons p_action)
{
    if (p_action & Qt::LeftButton) {
        QPointF local_pos = mapFromScene(p_scene_pos);
        QPainterPath new_path = path();

        int grabber_index = grabbers_.indexOf(grabber);

        new_path.setElementPositionAt(grabber_index, local_pos.x(), local_pos.y());
        setPath(new_path);
        setGrabbersPositions();
    }
}

void VeShapePolyline::doOnGrabberRelease(VeGrabberDot *grabber)
{
    QPointF local_pos = mapFromScene(grabber->scenePos());
    QPainterPath old_path = path();
    QPainterPath new_path;
    QLineF points_distance;
    int must_be_deleted = -1;

    int grabber_index = grabbers_.indexOf(grabber);
    points_distance.setP1(old_path.elementAt(grabber_index));

    if (grabber_index >= 0 && grabber_index < old_path.elementCount() - 1) {
        points_distance.setP2(old_path.elementAt(grabber_index + 1));
        must_be_deleted = (points_distance.length() <= k_min_point_distance_)? grabber_index + 1 : must_be_deleted;
    }
    if (grabber_index > 0 && grabber_index <= old_path.elementCount() - 1) {
        points_distance.setP2(old_path.elementAt(grabber_index - 1));
        must_be_deleted = (points_distance.length() <= k_min_point_distance_)? grabber_index - 1 : must_be_deleted;
    }
    if (must_be_deleted >= 0 && old_path.elementCount() > 2) {
        new_path.moveTo(old_path.elementAt(0));
        for(int i = 1; i < old_path.elementCount() - 1; i++) {
            new_path.lineTo(old_path.elementAt((i < grabber_index)? i: i + 1));
        }
        delete grabbers_[must_be_deleted];
        grabbers_.removeAt(must_be_deleted);
    } else {
        new_path = old_path;
        new_path.setElementPositionAt(grabber_index, local_pos.x(), local_pos.y());
    }
    setPath(new_path);
    setGrabbersPositions();
}

int VeShapePolyline::grabbersCount()
{
    return path().elementCount();
}

bool VeShapePolyline::pointToLineProjection(const QPointF &p_point, const QLineF &p_line, QPointF *p_projection_point) const
{
    QLineF check_line;

    check_line.setP1(p_point);
    check_line.setP2(QPointF(p_point.x() + 10, p_point.x() +10));

    check_line.setAngle(check_line.angle() + check_line.angleTo(p_line) - 90);
    bool is_bounded_intersection = p_line.intersects(check_line, p_projection_point)  == QLineF::BoundedIntersection;
    if (*p_projection_point == p_point && is_bounded_intersection) {

        return true;

    } else {
        check_line.setP2(*p_projection_point);
        check_line.setLength(check_line.length() + 2);

        return p_line.intersects(check_line, p_projection_point) == QLineF::BoundedIntersection;
    }
}
