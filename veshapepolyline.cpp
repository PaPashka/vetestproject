#include "veshapepolyline.h"

VeShapePolyline::VeShapePolyline(QObject *parent)
    : VeShapeItem(parent)
{

}

int VeShapePolyline::type() const
{
    return Type;
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

void VeShapePolyline::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::LeftButton && grabbers_[0]->isVisible()) {
        QPointF local_pos = mapFromScene(event->scenePos());
        QPainterPath old_path = path();
        QPainterPath new_path;
        QLineF first_line(local_pos.x() - 2, local_pos.y() - 2, local_pos.x() + 2, local_pos.y() + 2);
        QLineF second_line(local_pos.x() + 2, local_pos.y() - 2, local_pos.x() - 2, local_pos.y() + 2);
        QLineF polyline_element;
        bool new_point_added = false;

        new_path.moveTo(old_path.elementAt(0));
        for(int i = 1; i < old_path.elementCount(); i++) {
            if (new_point_added) {
                new_path.lineTo(old_path.elementAt(i));
            } else {
                polyline_element = QLineF(old_path.elementAt(i - 1), old_path.elementAt(i));
                if (polyline_element.intersects(first_line, nullptr) == QLineF::BoundedIntersection||
                    polyline_element.intersects(second_line, nullptr) == QLineF::BoundedIntersection) {

                    new_path.lineTo(local_pos);
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
    int must_be_deleted = -1;

    int grabber_index = grabbers_.indexOf(grabber);

    if (grabber_index >= 0 && grabber_index < old_path.elementCount() - 1) {
        must_be_deleted = (old_path.elementAt(grabber_index + 1).x == local_pos.x() &&
                           old_path.elementAt(grabber_index + 1).y == local_pos.y())? grabber_index + 1 : must_be_deleted;
    }
    if (grabber_index > 0 && grabber_index <= old_path.elementCount() - 1) {
        must_be_deleted = (old_path.elementAt(grabber_index - 1).x == local_pos.x() &&
                           old_path.elementAt(grabber_index - 1).y == local_pos.y())? grabber_index - 1 : must_be_deleted;
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
