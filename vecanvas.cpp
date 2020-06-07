#include "vecanvas.h"

VeCanvas::VeCanvas(QObject *parent)
    : QGraphicsScene(parent)
    , current_tool_(TOOL_SEL_MOVE_TRANS)
    , current_action_(ACT_SELECTION)
    , selected_item_(nullptr)
    , is_action_in_progress_(false)
    , current_brush_(Qt::transparent)
    , current_pen_(Qt::black, 1)
    , top_item_under_cursor_(nullptr)
{

}

void VeCanvas::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        switch (current_action_) {
            case ACT_DRAW_ITEM: {

                switch (current_tool_) {
                    case TOOL_DRAW_RECTANGLE: {
                        selected_item_ = new VeShapeRect(this);
                        break;
                    }
                    case TOOL_DRAW_POLY_LINE: {
                        selected_item_ = new VeShapePolyline(this);
                        break;
                    }
                    default: {
                        selected_item_ = nullptr;
                        break;
                    }
                }

                if (selected_item_) {
                    addItem(dynamic_cast<QGraphicsItem *>(selected_item_));
                    start_point_ = event->scenePos();
                    selected_item_->setBrush(getCurrentBrush());
                    selected_item_->setPen(getCurrentPen());                    

                    selected_item_->initizlizeShape(start_point_, event->scenePos());
                    is_action_in_progress_ = true;
                }
                break;
            }
            case ACT_SELECTION: {
                QGraphicsItem *top_item_under_cursor = this->itemAt(event->scenePos(), QTransform());
                setItemSelected(top_item_under_cursor);
                if (dynamic_cast<VeGrabberDot *>(top_item_under_cursor)) {

                    current_action_ = ACT_TRANSFORM;
                    QGraphicsScene::mousePressEvent(event);

                } else if (selected_item_) {
                    current_action_ = ACT_MOVE_ITEM;
                } else {
                    current_action_ = ACT_MOVE_VIEW;
                    start_point_ = event->screenPos();
                }
                is_action_in_progress_ = true;
                break;
            }
            default: {            
                QGraphicsScene::mousePressEvent(event);                
                break;                
            }
        }
        emit itemUnderCursorChanged(nullptr);
        update();
    } else {
        QGraphicsScene::mousePressEvent(event);        
    }
}

void VeCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (is_action_in_progress_) {
        switch (current_action_) {
            case ACT_DRAW_ITEM: {
                selected_item_->initizlizeShape(start_point_, event->scenePos());
                break;                
            }
            case ACT_MOVE_ITEM: {
                selected_item_->moveBy(event->scenePos() - event->lastScenePos());
                break;
            }
            case ACT_MOVE_VIEW: {
                QRectF rect = sceneRect();
                setSceneRect(rect.x() - event->screenPos().x() + event->lastScreenPos().x(),
                             rect.y() - event->screenPos().y() + event->lastScreenPos().y(),
                             rect.width(),
                             rect.height());
                break;
            }
            default: {
                QGraphicsScene::mouseMoveEvent(event);
                break;
            }
        }
    } else {
        QGraphicsItem *top_item = itemAt(event->scenePos(), QTransform());
        if (top_item_under_cursor_ != top_item && !(dynamic_cast<VeGrabberDot *>(top_item_under_cursor_) != nullptr &&
                                                    event->buttons() & Qt::RightButton)) {
            top_item_under_cursor_ = (top_item == dynamic_cast<QGraphicsItem *>(selected_item_))? nullptr : top_item;
            emit itemUnderCursorChanged(top_item_under_cursor_);
            update();
        }

        QGraphicsScene::mouseMoveEvent(event);
    }
}

void VeCanvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::LeftButton && is_action_in_progress_) {        
        is_action_in_progress_ = false;

        switch (current_action_) {
            case ACT_DRAW_ITEM: {
                selected_item_ = nullptr;
                break;
            }
            case ACT_MOVE_ITEM: {
                current_action_ = ACT_SELECTION;
                break;
            }
            case ACT_MOVE_VIEW: {
                current_action_ = ACT_SELECTION;
                break;
            }
            case ACT_TRANSFORM: {
                current_action_ = ACT_SELECTION;
                break;
            }
            default: {
                selected_item_ = nullptr;
                QGraphicsScene::mouseReleaseEvent(event);
                break;
            }
        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void VeCanvas::keyPressEvent(QKeyEvent *event)
{
    if (!is_action_in_progress_ && event->key() == Qt::Key_Delete && selected_item_) {
        removeItem(dynamic_cast<QGraphicsItem *>(selected_item_));
        delete selected_item_;
        selected_item_ = nullptr;
        top_item_under_cursor_ = nullptr;
        setItemSelected(nullptr);
    }
}

// true     - если произошла смена выбора объекта
// false    - если выбор остался прежний
bool VeCanvas::setItemSelected(QGraphicsItem *p_item)
{
    if (dynamic_cast<VeShapeItem *>(p_item) != selected_item_ && qgraphicsitem_cast<VeGrabberDot *>(p_item) == nullptr) {

        selected_item_ = dynamic_cast<VeShapeItem *>(p_item);
        if (selected_item_) {
            current_brush_ = selected_item_->brush();
            current_pen_ = selected_item_->pen();
        }
        emit itemSelected(selected_item_);
        return true;

    } else {
        return false;
    }
}

int VeCanvas::svgItemNameToShapeType(const QString &p_name)
{
    if (p_name == "rect") return VeShapeItem::RECTANGLE;
    if (p_name == "path") return VeShapeItem::POLYLINE;
    return 0;
}

void VeCanvas::setCurrentBrush(const QBrush &p_brush)
{
    current_brush_ = p_brush;
    if (selected_item_) {
        selected_item_->setBrush(current_brush_);
    }
}

QBrush VeCanvas::getCurrentBrush() const
{
    return current_brush_;
}

void VeCanvas::setCurrentPen(const QPen &p_pen)
{
    current_pen_ = p_pen;
    if (selected_item_) {
        selected_item_->setPen(current_pen_);
    }
}

QPen VeCanvas::getCurrentPen() const
{
    return current_pen_;
}

void VeCanvas::saveToSVG(const QString &p_file)
{
    QSvgGenerator generator;
    QRectF saving_rect = itemsBoundingRect();

    generator.setFileName(p_file);
    generator.setSize(QSize(saving_rect.width(), saving_rect.height()));
    generator.setViewBox(saving_rect);

    setItemSelected(nullptr);

    QPainter painter;
    painter.begin(&generator);
    render(&painter, saving_rect, saving_rect);
    painter.end();
}

void VeCanvas::loadFromSVG(const QString &p_file)
{
    foreach (QGraphicsItem *item, items()) {
        selected_item_ = nullptr;
        top_item_under_cursor_ = nullptr;
        removeItem(dynamic_cast<QGraphicsItem *>(item));
        delete item;
    }
    setSceneRect(0, 0, width(), height());

    QDomDocument svg_doc;
    QFile file(p_file);

    if (file.open(QIODevice::ReadOnly) && svg_doc.setContent(&file)) {

        QDomNodeList gList = svg_doc.elementsByTagName("g");

        for (int i = 0; i < gList.size(); i++) {
            QDomNode gNode = gList.item(i);
            QDomElement element = gNode.firstChildElement();
            QDomElement element_propertys = gNode.toElement();

            VeShapeItem *new_item = nullptr;

            switch (svgItemNameToShapeType(element.tagName())) {
                case VeShapeItem::RECTANGLE: {
                    new_item = new VeShapeRect(this);
                    break;
                }
                case VeShapeItem::POLYLINE: {
                    new_item = new VeShapePolyline(this);
                    break;
                }
                default: {
                    new_item = nullptr;
                    break;
                }
            }
            if (new_item) {
                QColor item_brush_color(element_propertys.attribute("fill", "#ffffff"));
                item_brush_color.setAlphaF(element_propertys.attribute("fill-opacity", "0").toFloat());
                new_item->setBrush(QBrush(item_brush_color));

                QColor item_pen_color(element_propertys.attribute("stroke", "#000000"));
                item_pen_color.setAlphaF(element_propertys.attribute("stroke-opacity", "0").toFloat());
                new_item->setPen(QPen(item_pen_color, element_propertys.attribute("stroke-width", "0").toInt()));

                QTransform item_transform;

                QString transform_string = element_propertys.attribute("transform");
                if (transform_string.startsWith("matrix")) {
                    transform_string.replace("matrix(", "");
                    transform_string.replace(")", "");
                    QStringList transform_list = transform_string.split(",");

                    item_transform.setMatrix(transform_list[0].toFloat(),
                                             transform_list[1].toFloat(),
                                             item_transform.m13(),
                                             transform_list[2].toFloat(),
                                             transform_list[3].toFloat(),
                                             item_transform.m23(),
                                             transform_list[4].toFloat(),
                                             transform_list[5].toFloat(),
                                             item_transform.m33());
                }

                if (new_item->fromSvgElement(element, item_transform) == 0) {
                    addItem(dynamic_cast<QGraphicsItem *>(new_item));
                } else {
                    delete new_item;
                }
            }
        }
    }
    file.close();
}

void VeCanvas::setCurrentTool(int p_tool)
{
    current_tool_ = p_tool;
    if (current_tool_ != TOOL_SEL_MOVE_TRANS) {
        setItemSelected(nullptr);
    }
    switch (current_tool_) {
        case TOOL_SEL_MOVE_TRANS: {
            current_action_ = ACT_SELECTION;
            break;
        }
        case TOOL_DRAW_RECTANGLE:
        case TOOL_DRAW_POLY_LINE: {
            current_action_ = ACT_DRAW_ITEM;
            break;
        }
        default: {
            current_action_ = TOOL_SEL_MOVE_TRANS;
            break;
       }
    }
}

int VeCanvas::getCurrentTool() const
{
    return current_action_;
}
