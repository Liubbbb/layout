#include "customitem.h"

CustomItem::CustomItem(StWindowInf windowInf,const QRectF & rect,QGraphicsItem *parent )
    :QGraphicsItem(parent)
{
    iRect = rect;  //iRect的大小发生改变会导致重绘
    itemInf = windowInf;  //存储item对应的属性
    setDraggingEdge();
    //设置item自身属性
    setFlag(QGraphicsItem::ItemIsMovable, true);  // item可拖动
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}

//为矩形缩放设置缩放点或边
void CustomItem::setDraggingEdge()
{
    for (int i = DragingRectItem::LeftTop; i <= DragingRectItem::Left; ++i) {
        DragingRectItem *dItem = new DragingRectItem(this,iRect,static_cast<DragingRectItem::Direction>(i));
        dRects.push_back(dItem); //shr相当于item，存储在m_handles向量里，是伸缩点的原型
    }
    const QRectF &rect = this->boundingRect(); //获取item
    const DragingRect::iterator hend =  dRects.end();
    //在vector里遍历小矩形并将其移动到各个点的位置
    for (DragingRect::iterator it = dRects.begin(); it != hend; ++it) {
        DragingRectItem *hndl = *it;
        switch (hndl->dir()) {
        case DragingRectItem::LeftTop:  //设置左上角的点伸缩点
            hndl->move(rect.x() - 2, rect.y() - 2);
            break;
        case DragingRectItem::Top:
            hndl->move(rect.x() + 3, rect.y() - 2);
            break;
        case DragingRectItem::RightTop:
            hndl->move(rect.x() + rect.width() - 2, rect.y() - 2);
            break;
        case DragingRectItem::Right:
            hndl->move(rect.x() + rect.width() - 2, rect.y() + 3);
            break;
        case DragingRectItem::RightBottom:
            hndl->move(rect.x() + rect.width() - 2, rect.y() + rect.height() - 2);
            break;
        case DragingRectItem::Bottom:
            hndl->move(rect.x() + 3, rect.y() + rect.height() - 2);
            break;
        case DragingRectItem::LeftBottom:
            hndl->move(rect.x() - 2, rect.y() + rect.height() - 2);
            break;
        case DragingRectItem::Left:
            hndl->move(rect.x() - 2, rect.y() + 3);
            break;
        default:
            break;
        }
    }
}

void CustomItem::settingSizeTo(DragingRectItem::Direction dir, const QPointF &point)
{
    QPointF local = mapFromScene(point);
    QString dirName;

    QRectF delta =iRect.toRect();
    switch (dir) {
    case DragingRectItem::LeftTop:
        delta.setTopLeft(local.toPoint()); //改变item左上角的点 但其右下角不变
        break;
    case DragingRectItem::Top:
        dirName = "Top";
        delta.setTop(local.y());
        break;

    case DragingRectItem::RightTop:
        dirName = "RightTop";
        delta.setTopRight(local.toPoint());
        break;

    case DragingRectItem::Left:
        dirName = "Left";
        delta.setLeft(local.x());
        break;
    case DragingRectItem::Right:
        dirName = "Rigth";
        delta.setRight(local.x());
        break;

    case DragingRectItem::LeftBottom:
        dirName = "LeftBottom";
        delta.setBottomLeft(local.toPoint());
        break;

    case DragingRectItem::Bottom:
        dirName = "Bottom";
        delta.setBottom(local.y());
        break;

    case DragingRectItem::RightBottom:
        dirName = "RightBottom";
        delta.setBottomRight(local.toPoint());
        break;

    default:
        break;
    }

    prepareGeometryChange();
    iRect =delta; //boundingRect()和paint()中的m_rect也会变化
    setDraggingEdge();
}

DragingRectItem::Direction CustomItem::hitTest(const QPointF &point) const
{
    const DragingRect::const_iterator hend =  dRects.end();
    for (DragingRect::const_iterator it = dRects.begin(); it != hend; ++it)
    {
        if ((*it)->hitTest(point) ){
            return (*it)->dir();  //每个伸缩点item对象都对应一个dir()
        }
    }
    return DragingRectItem::None;
}

Qt::CursorShape CustomItem::getCursor(DragingRectItem::Direction dir)
{
    switch (dir) {
    case DragingRectItem::Right:
        return Qt::SizeHorCursor;
    case DragingRectItem::RightTop:
        return Qt::SizeBDiagCursor;
    case DragingRectItem::RightBottom:
        return Qt::SizeFDiagCursor;
    case DragingRectItem::LeftBottom:
        return Qt::SizeBDiagCursor;
    case DragingRectItem::Bottom:
        return Qt::SizeVerCursor;
    case DragingRectItem::LeftTop:
        return Qt::SizeFDiagCursor;
    case DragingRectItem::Left:
        return Qt::SizeHorCursor;
    case DragingRectItem::Top:
        return Qt::SizeVerCursor;
    default:
        break;
    }
    return Qt::ArrowCursor;
}


void CustomItem::isSelectItem(bool select)
{
    isSelect = select;
}

QRectF CustomItem::boundingRect() const
{
    return iRect;  //基于该矩形边界进行操作和显示，若该矩形位置或长宽发生变化，该函数和paint()函数会被调用，重新绘画一个新的item
}

void CustomItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPen pen;
    pen.setColor(Qt::white);
    if(isSelect)
        {
           pen.setColor(Qt::magenta);
        }
     painter->setPen(pen);
     painter->drawRect(iRect);
}

void CustomItem::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    scene()->clearSelection();
    setSelected(true);
    isSelectItem(true);
}

QVariant CustomItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if ((change == ItemPositionChange || change == ItemPositionHasChanged) && scene()) // 控件发生移动
    {
        QPointF newPos = value.toPointF();  //相对于左上角的位置
        QRectF rect = scene()->sceneRect();
        if(!rect.contains(newPos)) //左上角
        {
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }
        QRectF thisRectF = boundingRect();
        QPointF nowPos = QPointF(newPos.x() + thisRectF.width(),newPos.y());
        if(!rect.contains(nowPos))//右上角
        {
            newPos.setX(rect.width() - thisRectF.width());
            this->setPos(newPos);
            return newPos;
        }
        nowPos = QPointF(newPos.x(),newPos.y() + thisRectF.height());
        if(!rect.contains(nowPos))//左下角
        {
           newPos.setY(rect.height() - thisRectF.height());
           this->setPos(newPos);
           return newPos;
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

