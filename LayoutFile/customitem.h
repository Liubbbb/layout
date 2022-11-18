#ifndef CUSTOMITEM_H
#define CUSTOMITEM_H
#include <QGraphicsScene>
#include "dragingrectitem.h"
#include <QDebug>
#include "publicdefine.h"
#include <QGraphicsSceneMouseEvent>
#include <QLabel>

class CustomItem : public QGraphicsItem
{
public:

    CustomItem(StWindowInf windowInf,const QRectF & rect,QGraphicsItem *parent = nullptr);

    //为矩形框设置拖动点/边
    void setDraggingEdge();
    //重设item大小
    virtual void settingSizeTo(DragingRectItem::Direction dir, const QPointF & point );
    virtual Qt::CursorShape getCursor(DragingRectItem::Direction dir );
//    void move(const QPointF point);
    //检测按压
    DragingRectItem::Direction  hitTest( const QPointF & point ) const;
    // 重写item类型，以区分不同类型的item
    void isSelectItem(bool select);

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    StWindowInf itemInf;
    QRectF iRect;  //存储要绘画的窗口矩形

private:
    typedef QVector<DragingRectItem*> DragingRect;
    DragingRect dRects;
    bool isSelect = false;  // item是否被选中

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

};

#endif // CUSTOMITEM_H
