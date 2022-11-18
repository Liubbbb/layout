#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include "customitem.h"
#include "multiview.h"
#include "ui_MainEdit.h"


#include <QGraphicsView>
#include <QLineEdit>
#include <QMessageBox>

enum SelectMode
{
    none,
    move, //移动
    size, //改变大小
};

class CustomScene : public QGraphicsScene
{
public:
    CustomScene(QString layoutName,QGraphicsScene *parent = nullptr);
    void uiInit(Ui::MainEdit *ui_ptr);
    bool findSelectItem();
    void addWindowInScene(qreal width,qreal height,StWindowInf windowInf);
    void deleteWindowInScene();
    void setCursor(const QCursor & cursor );
    Qt::CursorShape currentCursor();
    bool findIndex(StLayoutInf sLayoutInf,int index);
    void deleteSameIndexItem(int index);
    SDVoEAPIRESULT saveLayout();
    
private:
    Ui::MainEdit *s_ui;
    CustomItem *item;
    QString layoutName;
    CustomItem *select = nullptr;
    CustomItem *beforItem = nullptr; //上一次被选中的item
    QRectF sRect;  //根据参数得到的窗口矩形
    MultiView mul;
    QVector<CustomItem*> itemsList; //存储scene上所有的item
    //判断当前鼠标所在方向
    DragingRectItem::Direction nDragHandle = DragingRectItem::None;
    SelectMode selectMode = none;
    qreal wStretch = 0;
    qreal hStretch = 0;
    int horizonValue;
    int widthValue;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // CUSTOMSCENE_H
