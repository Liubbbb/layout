#include "layoutinf.h"
#include "ui_layoutinf.h"
#include <QDebug>

LayoutInf::LayoutInf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayoutInf)
{
    ui->setupUi(this);

    this->setWindowTitle("layout information");
}

LayoutInf::~LayoutInf()
{
    delete ui;
}

void LayoutInf::getLayoutInf(StLayoutInf layoutInf)
{

    ui->nameEdit->setText(layoutInf.layout_name);
    ui->wEdit->setText(QString::number(layoutInf.width));
    ui->hEdit->setText(QString::number(layoutInf.height));
    ui->readEdit->setText(QString::number(layoutInf.read_only));
    if(layoutInf.sfObj.count()>0)
    {
        QJsonArray sArray;
        for (int i = 0;i<layoutInf.sfObj.count();i++) {
            QJsonObject sObj;
            sObj.insert("index",layoutInf.sfObj.at(i).index);
            sObj.insert("width",layoutInf.sfObj.at(i).width);
            sObj.insert("height",layoutInf.sfObj.at(i).height);
            sObj.insert("horizontal_position",layoutInf.sfObj.at(i).horizontal_position);
            sObj.insert("vertical_position",layoutInf.sfObj.at(i).vertical_position);
            sArray.append(sObj);
        }
        QJsonObject sObj;
        sObj.insert("surfaces",sArray);
        QJsonDocument doc(sObj);
        QByteArray byte = doc.toJson();
        ui->surfaceEdit->setText(byte);
    }else  ui->surfaceEdit->setText("Don't find any surface information!");

    if(layoutInf.wdObj.count()>0)
    {
        QJsonArray wArray;
        for(int i = 0;i<layoutInf.wdObj.count();i++){
            QJsonObject wObj;
            wObj.insert("index",layoutInf.wdObj.at(i).index);
            wObj.insert("width",layoutInf.wdObj.at(i).width);
            wObj.insert("height",layoutInf.wdObj.at(i).height);
            wObj.insert("horizontal_position",layoutInf.wdObj.at(i).horizontal_position);
            wObj.insert("vertical_position",layoutInf.wdObj.at(i).vertical_position);
            wObj.insert("content",layoutInf.wdObj.at(i).content);
            wObj.insert("target_surface",layoutInf.wdObj.at(i).target_surface);
            wObj.insert("horizontal_offset",layoutInf.wdObj.at(i).horizontal_offset);
            wObj.insert("vertical_offset",layoutInf.wdObj.at(i).vertical_offset);
            wArray.append(wObj);
        }
        QJsonObject wObj;
        wObj.insert("windows",wArray);
        QJsonDocument doc(wObj);
        QByteArray byte = doc.toJson();
        ui->windowEdit->setText(byte);

    }else  ui->windowEdit->setText("Don't find any window information!");

}
