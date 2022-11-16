#include "customscene.h"

CustomScene::CustomScene(QString layout,QGraphicsScene*parent)
    : QGraphicsScene(parent)
{
    layoutName = layout;
}

void CustomScene::uiInit(Ui::MainEdit *ui_ptr)
{
    s_ui = ui_ptr;
}

void CustomScene::addWindowInScene(qreal widthScale,qreal heightScale,StWindowInf windowInf)
{
    this->clearSelection();
    //大小的比例
    wStretch = widthScale;
    hStretch = heightScale;

    sRect = QRectF(QPointF(0,0),QSizeF(windowInf.width/wStretch, windowInf.height/hStretch));
    item = new CustomItem(windowInf,sRect,nullptr);
    item->setPos(windowInf.horizontal_position/wStretch, windowInf.vertical_position/hStretch);
    int value = 32-windowInf.index;  //index越小，value值越大，层级越在上面
    item->setZValue(value);
    item->itemInf.layoutName = layoutName;
    addItem(item);
    itemsList.append(item);
    item->setSelected(true);
    update();
}

void CustomScene::setCursor(const QCursor &cursor)
{
    QList<QGraphicsView*> views  = this->views();
    if ( views.count() > 0 ){
        QGraphicsView * view = views.first();
        view->setCursor(cursor);
    }
}
//执行删除窗口命令并在scene中删除item
void CustomScene::deleteWindowInScene()
{
    if (findSelectItem())
        {
            removeItem(select);
            for(int i = 0;i<itemsList.count();i++)
            {
                if(itemsList.at(i) == select)
                    itemsList.remove(i);
            }
            //清空左侧显示栏数据
            s_ui->indexEdit->clear();
            s_ui->horizonPosEdit->clear();
            s_ui->verticalPosEdit->clear();
            s_ui->widthEdit->clear();
            s_ui->heightEdit->clear();
            StWindowInf window_inf;
            mul.deleteWindow(layoutName,select->itemInf.index);
            removeItem(select);
            this->clearSelection();
            update();
        }

}


bool CustomScene::findSelectItem()
{
    QList<QGraphicsItem *> items= this->selectedItems();
    if (items.count() == 1)
    {
        select = qgraphicsitem_cast<CustomItem*>(items.first());
        return true;
    }
    return false;
}

void CustomScene::deleteSameIndexItem(int index)
{
    //遍历列表找到index相同的item,从list中remove并removeItem
    for(int i = 0;i<itemsList.count();i++)
    {
        if(itemsList.at(i)->itemInf.index == index)
         {
            removeItem(itemsList.at(i));
            itemsList.remove(i);
            this->clearSelection();
            update();
         }
    }
}

Qt::CursorShape CustomScene::currentCursor()
{
    QList<QGraphicsView*> views  = this->views();
    QGraphicsView * view = views.first();
    return view->cursor().shape() ;
}

bool CustomScene::findIndex(StLayoutInf sLayoutInf,int index)
{
    int windowNums = sLayoutInf.wdObj.count();
    for(int i=0; i<windowNums;i++)
    {
        if(sLayoutInf.wdObj.at(i).index == index)
            return true;
    }
    return false;
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return;
    if (currentCursor() == Qt::ArrowCursor)
        QGraphicsScene::mousePressEvent(event);
    if (findSelectItem())
    {
//            qDebug()<<"item的pos："<<select->scenePos(); //显示此时的位置
       s_ui->indexEdit->setText(QString::number(select->itemInf.index));
       s_ui->horizonPosEdit->setText(QString::number(qRound(select->scenePos().x()*wStretch)));
       s_ui->verticalPosEdit->setText(QString::number(qRound(select->scenePos().y()*hStretch)));
       QRect rect =select->iRect.toRect();
       s_ui->widthEdit->setText(QString::number(qRound(rect.width()*wStretch)));
       s_ui->heightEdit->setText(QString::number(qRound(rect.height()*hStretch)));

       if (beforItem == nullptr) beforItem = select;
       else if(select!= beforItem)
       {
           beforItem->isSelectItem(false);
           beforItem = select;
       }
//       qDebug()<<"item的boundingRect"<<select->boundingRect(); //item局部坐标
       nDragHandle = select->hitTest(event->scenePos());
       if (nDragHandle !=DragingRectItem::None)
            selectMode = size; //若不在伸缩范围内
       else
            selectMode =  move;  //意味着要进行移动
    }
    if(selectMode == move || selectMode == none)
        QGraphicsScene::mousePressEvent(event);
    update();
}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(findSelectItem()){
       if (nDragHandle != DragingRectItem::None && selectMode == size )
       {
           select->settingSizeTo(nDragHandle,event->scenePos());
           //同步更新右侧窗口信息
           QRect rect = select->iRect.toRect();
           s_ui->horizonPosEdit->setText(QString::number(qRound(select->scenePos().x()*wStretch)));
           s_ui->verticalPosEdit->setText(QString::number(qRound(select->scenePos().y()*hStretch)));
           s_ui->widthEdit->setText(QString::number(qRound(rect.width()*wStretch)));
           s_ui->heightEdit->setText(QString::number(qRound(rect.height()*hStretch)));
       }
       //移动时，按压前，nDragHandle和selectMode均为null
       else if(nDragHandle == DragingRectItem::None && selectMode == none )
       {
            DragingRectItem::Direction handle = select->hitTest(event->scenePos());
            if ( handle != DragingRectItem::None){
                setCursor(select->getCursor(handle));
            }else{
                setCursor(Qt::ArrowCursor);
            }
       }
       else if(nDragHandle == DragingRectItem::None && selectMode == move )
       {
           QGraphicsScene::mouseMoveEvent(event);
           s_ui->horizonPosEdit->setText(QString::number(qRound(select->scenePos().x()*wStretch)));
           s_ui->verticalPosEdit->setText(QString::number(qRound(select->scenePos().y()*hStretch)));
       }

   }
    update();
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return;
    if(findSelectItem())
    {
        //此处添加左侧更新item.scenPos
        QRect rect = select->iRect.toRect();
        qDebug()<<"item现在的位置："<<select->scenePos();
        s_ui->horizonPosEdit->setText(QString::number(qRound(select->scenePos().x()*wStretch)));
        s_ui->verticalPosEdit->setText(QString::number(qRound(select->scenePos().y()*hStretch)));
        s_ui->widthEdit->setText(QString::number(qRound(rect.width()*wStretch)));
        s_ui->heightEdit->setText(QString::number(qRound(rect.height()*hStretch)));

        //因为窗口位置信息发生改变，释放鼠标后要发送创建窗口命令
        select->itemInf.horizontal_position = qRound(select->scenePos().x()*wStretch); //更新item信息
        select->itemInf.vertical_position = qRound(select->scenePos().y()*hStretch);
        select->itemInf.width = qRound(rect.width()*wStretch);
        select->itemInf.height = qRound(rect.height()*hStretch);
        qDebug()<<"x:"<<select->itemInf.horizontal_position;
        qDebug()<<"y:"<<select->itemInf.vertical_position;
        qDebug()<<"w:"<<select->itemInf.width;
        qDebug()<<"h:"<<select->itemInf.height;
        qDebug()<<"name:"<<select->itemInf.layoutName;
        mul.createWindow(select->itemInf); // 发送创建窗口命令 = 更新窗口
    }
    setCursor(Qt::ArrowCursor);
    selectMode = none;
    nDragHandle = DragingRectItem::None;
    QGraphicsScene::mouseReleaseEvent(event);
}
