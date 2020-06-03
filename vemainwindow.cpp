#include "vemainwindow.h"
#include "ui_vemainwindow.h"

VeMainWindow::VeMainWindow(QWidget *parent)
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

    scene_ = new VeCanvas(this);
    scene_->setSceneRect(ui->graphicsView->rect());
    connect(scene_, SIGNAL(itemSelected(const VeShapeItem *)), this, SLOT(slotForVEShapeItem_selected(const VeShapeItem *)));
    ui->graphicsView->setScene(scene_);
}

VeMainWindow::~VeMainWindow()
{
    delete ui;
}

void VeMainWindow::slotForToolBtn_clicked()
{
    scene_->setAction(toolsGroup_->checkedId());
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

void VeMainWindow::slotForVEShapeItem_selected(const VeShapeItem *p_item)
{    
    if (p_item) {
        ui->pushBtnBrushColor->setStyleSheet("border: 2px solid #000000;"
                                             "border-radius: 0px;"
                                             "background-color: " + p_item->brush().color().name(QColor::HexArgb) + ";");
        ui->spinBoxPenWidth->setValue(p_item->pen().width());
    }
}

void VeMainWindow::on_pushBtnBrushColor_clicked()
{
    QBrush new_brush = scene_->getCurrentBrush();
    QColor selected_color = QColorDialog::getColor(new_brush.color(), this, tr("Select Color"), QColorDialog::ShowAlphaChannel);
    if (selected_color.isValid()) {
        new_brush.setColor(selected_color);
        scene_->setCurrentBrush(new_brush);
        ui->pushBtnBrushColor->setStyleSheet("border: 2px solid #000000;"
                                             "border-radius: 0px;"
                                             "background-color: " + selected_color.name(QColor::HexArgb) + ";");
    }
}

void VeMainWindow::on_spinBoxPenWidth_valueChanged(int arg1)
{
    QPen new_pen = scene_->getCurrentPen();
    new_pen.setWidth(arg1);
    scene_->setCurrentPen(new_pen);
}

void VeMainWindow::on_pushBtnSave_clicked()
{
    QString file_path = QFileDialog::getSaveFileName(this, tr("Save SVG"), "", tr("SVG files (*.svg)"));

    if (!file_path.isEmpty()){
        scene_->saveToSVG(file_path);
    }
}

void VeMainWindow::on_pushBtnLoad_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open SVG"), "", tr("SVG files (*.svg)"));

    if (!file_path.isEmpty()){
        scene_->loadFromSVG(file_path);
    }
}
