#include "newlayoutwidget.h"
#include "ui_newlayoutwidget.h"
#include <LayoutFile/layout.h>
#include <QMessageBox>

newlayoutwidget::newlayoutwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::newlayoutwidget)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags (Qt::CustomizeWindowHint);
//    setWindowFlags (Qt::FramelessWindowHint);
}

newlayoutwidget::~newlayoutwidget()
{
    delete ui;
}


void newlayoutwidget::on_addBtn_clicked()
{
    QString layout_name = ui->layoutName->text();
    int height = ui->height->text().toInt();
    int width = ui->width->text().toInt();
    if(layout_name==nullptr) return;

    MultiView mul;
    SDVoEAPIRESULT result = mul.createLayout(layout_name,width,height);

    QString str;
    if(result == 0) {str = "success";updateNew = true;}
    QMessageBox *message = new QMessageBox();
    message->information(this, "result",str);

}

void newlayoutwidget::on_closeBtn_clicked()
{
    emit closeWidget(updateNew);
    close();
}
