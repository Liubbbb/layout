#include "dragingrectitem.h"

DragingRectItem::DragingRectItem(QGraphicsItem* parent ,QRectF rect,Direction dir)
    :QGraphicsItem(parent),
      iDirect(dir)
{
    switch (dir)
    {
    case LeftTop:
        dRect = QRectF(QPointF(0, 0),QSizeF(4, 4));
        break;
    case Top:
        dRect = QRectF(QPointF(0, 0),QSizeF(rect.width()-6, 4));
        break;
    case RightTop:
        dRect = QRectF(QPointF(0, 0),QSizeF(4, 4));
        break;
    case Right:
        dRect = QRectF(QPointF(0, 0),QSizeF(4, rect.height()-6));
        break;
    case RightBottom:
        dRect = QRectF(QPointF(0, 0),QSizeF(4, 4));
        break;
    case Bottom:
        dRect = QRectF(QPointF(0, 0),QSizeF(rect.width()-6, 4));
        break;
    case LeftBottom:
        dRect = QRectF(QPointF(0, 0),QSizeF(4, 4));
       break;
    case Left:
        dRect = QRectF(QPointF(0, 0),QSizeF(4, rect.height()-6));
        break;
    default:
        break;
    }
    setParentItem(parent);
    this->setAcceptHoverEvents(true);
    hide();  //隐藏该item
}

DragingRectItem::Direction DragingRectItem::dir() const
{
    return iDirect; //返回伸缩方向
}

QRectF DragingRectItem::boundingRect() const
{
    return dRect;
}

void DragingRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawRect(dRect);
}

void DragingRectItem::move(qreal x, qreal y)
{
    setPos(x,y);
}

bool DragingRectItem::hitTest(const QPointF &point)
{
    QPointF pt = mapFromScene(point);
    return dRect.contains(pt);
}

//void DragingRectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
//{
//    hitTest(event->scenePos());
//}
