#include "vemainwindow.h"
#include "ui_vemainwindow.h"

VEMainWindow::VEMainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VEMainWindow)
{
    ui->setupUi(this);
    toolsGroup_ = new QButtonGroup;
    toolsGroup_->addButton(ui->toolBtnSelect);
    toolsGroup_->setId(ui->toolBtnSelect, VeCanvas::TOOL_SEL_MOVE_TRANS);
    toolsGroup_->addButton(ui->toolBtnPLine);
    toolsGroup_->setId(ui->toolBtnPLine, VeCanvas::TOOL_DRAW_POLY_LINE);
    toolsGroup_->addButton(ui->toolBtnRect);
    toolsGroup_->setId(ui->toolBtnRect, VeCanvas::TOOL_DRAW_RECTANGLE);
    toolsGroup_->exclusive();
    ui->toolBtnSelect->setChecked(true);
    foreach (QAbstractButton *tool_btn, toolsGroup_->buttons()) {
        connect(tool_btn, SIGNAL(clicked()), this, SLOT(slotForToolBtn_clicked()));
    }
    ui->pushBtnBrushColor->setStyleSheet("border: 2px solid black;"
                                         "border-radius: 0px;"
                                         "background-color: transparent;");
    ui->pushBtnPenColor->setStyleSheet("border: 1px solid black;"
                                         "border-radius: 0px;"
                                         "background-color: black;");

    scene_ = new VeCanvas(this);
    scene_->setSceneRect(ui->graphicsView->rect());
    connect(scene_, SIGNAL(itemSelected(const VeShapeItem *)), this, SLOT(slotForVEShapeItem_selected(const VeShapeItem *)));
    ui->graphicsView->setScene(scene_);
}

VEMainWindow::~VEMainWindow()
{
    delete ui;
}

void VEMainWindow::slotForToolBtn_clicked()
{
    scene_->setCurrentTool(toolsGroup_->checkedId());
    switch (toolsGroup_->checkedId()) {
        case VeCanvas::TOOL_SEL_MOVE_TRANS: {
            ui->graphicsView->setCursor(Qt::ArrowCursor);
            break;
        }
        case VeCanvas::TOOL_DRAW_RECTANGLE:
        case VeCanvas::TOOL_DRAW_POLY_LINE: {
            ui->graphicsView->setCursor(Qt::CrossCursor);
            break;
        }
        default: {
            ui->graphicsView->setCursor(Qt::ArrowCursor);
            break;
        }
    }
}

void VEMainWindow::slotForVEShapeItem_selected(const VeShapeItem *p_item)
{
    if (p_item) {
        ui->pushBtnBrushColor->setStyleSheet("border: 2px solid " + p_item->pen().color().name(QColor::HexArgb) + ";"
                                             "border-radius: 0px;"
                                             "background-color: " + p_item->brush().color().name(QColor::HexArgb) + ";");
        ui->pushBtnPenColor->setStyleSheet("border: 1px solid #000000;"
                                             "border-radius: 0px;"
                                             "background-color: " + p_item->pen().color().name(QColor::HexArgb) + ";");
        ui->spinBoxPenWidth->setValue(p_item->pen().width());
    }
}

void VEMainWindow::on_pushBtnBrushColor_clicked()
{
    QBrush new_brush = scene_->getCurrentBrush();
    QColor selected_color = QColorDialog::getColor(new_brush.color(), this, tr("Select Color"), QColorDialog::ShowAlphaChannel);
    if (selected_color.isValid()) {
        new_brush.setColor(selected_color);
        scene_->setCurrentBrush(new_brush);
        ui->pushBtnBrushColor->setStyleSheet("border: 2px solid " + scene_->getCurrentPen().color().name(QColor::HexArgb) + ";"
                                             "border-radius: 0px;"
                                             "background-color: " + selected_color.name(QColor::HexArgb) + ";");
    }
}

void VEMainWindow::on_spinBoxPenWidth_valueChanged(int arg1)
{
    QPen new_pen = scene_->getCurrentPen();
    new_pen.setWidth(arg1);
    scene_->setCurrentPen(new_pen);
}

void VEMainWindow::on_pushBtnSave_clicked()
{
    QString file_path = QFileDialog::getSaveFileName(this, tr("Save SVG"), "", tr("SVG files (*.svg)"));

    if (!file_path.isEmpty()){
        scene_->saveToSVG(file_path);
    }
}

void VEMainWindow::on_pushBtnLoad_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open SVG"), "", tr("SVG files (*.svg)"));

    if (!file_path.isEmpty()){
        scene_->loadFromSVG(file_path);
    }
}

void VEMainWindow::on_pushBtnPenColor_clicked()
{
    QPen new_pen = scene_->getCurrentPen();
    QColor selected_color = QColorDialog::getColor(new_pen.color(), this, tr("Select Color"), QColorDialog::ShowAlphaChannel);
    if (selected_color.isValid()) {
        new_pen.setColor(selected_color);
        scene_->setCurrentPen(new_pen);
        ui->pushBtnPenColor->setStyleSheet("border: 1px solid #000000;"
                                             "border-radius: 0px;"
                                             "background-color: " + selected_color.name(QColor::HexArgb) + ";");
        ui->pushBtnBrushColor->setStyleSheet("border: 2px solid " + selected_color.name(QColor::HexArgb) + ";"
                                             "border-radius: 0px;"
                                             "background-color: " + scene_->getCurrentBrush().color().name(QColor::HexArgb) + ";");
    }
}
