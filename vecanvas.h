#ifndef VECANVAS_H
#define VECANVAS_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <QCursor>
#include <QKeyEvent>
#include <QSvgGenerator>
#include <QPainter>
#include <QDomDocument>
#include "veshaperect.h"
#include "veshapepolyline.h"

class VeCanvas : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit VeCanvas(QObject *parent = nullptr);

    enum ToolsType {
        TOOL_SEL_MOVE_TRANS,
        TOOL_DRAW_POLY_LINE,
        TOOL_DRAW_RECTANGLE
    };

    enum ActionType {
        ACT_DRAW_ITEM,
        ACT_SELECTION,
        ACT_TRANSFORM,
        ACT_MOVE_ITEM,
        ACT_MOVE_VIEW
    };

    void setCurrentTool(int p_tool);
    int getCurrentTool() const;
    void setCurrentBrush(const QBrush &p_color);
    QBrush getCurrentBrush() const;
    void setCurrentPen(const QPen &p_color);
    QPen getCurrentPen() const;
    void saveToSVG(const QString &p_file);
    void loadFromSVG(const QString &p_file);

signals:
    void itemSelected(const VeShapeItem *p_item);
    void itemUnderCursorChanged(const QGraphicsItem *p_item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    int current_tool_;
    int current_action_;
    QPointF start_point_;
    VeShapeItem *selected_item_;
    bool is_action_in_progress_;
    QBrush current_brush_;
    QPen current_pen_;
    QGraphicsItem *top_item_under_cursor_;

    bool setItemSelected(QGraphicsItem *p_item);
    int svgItemNameToShapeType(const QString &p_name);

};

#endif // VECANVAS_H
