#ifndef VEMAINWINDOW_H
#define VEMAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QFrame>
#include <QGraphicsView>
#include <QButtonGroup>
#include <QGraphicsScene>
#include <QColorDialog>
#include <QFileDialog>
#include "vecanvas.h"

QT_BEGIN_NAMESPACE
namespace Ui { class VEMainWindow; }
QT_END_NAMESPACE

class VeMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit VeMainWindow(QWidget *parent = nullptr);
    ~VeMainWindow();

private:
    Ui::VEMainWindow *ui;
    QButtonGroup *toolsGroup_;
    VeCanvas *scene_;

private slots:
    void slotForToolBtn_clicked();
    void slotForVEShapeItem_selected(const VeShapeItem *p_item);
    void on_pushBtnBrushColor_clicked();
    void on_spinBoxPenWidth_valueChanged(int arg1);
    void on_pushBtnSave_clicked();
    void on_pushBtnLoad_clicked();
};
#endif // VEMAINWINDOW_H
