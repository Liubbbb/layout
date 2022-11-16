#ifndef DRAGINGRECTITEM_H
#define DRAGINGRECTITEM_H
#include <QGraphicsItem>
#include <QPainter>


class DragingRectItem : public QGraphicsItem
{
public:

    enum Direction { LeftTop , Top, RightTop, Right, RightBottom,
                     Bottom, LeftBottom, Left , Center, None};

    DragingRectItem(QGraphicsItem* parent ,QRectF rect,Direction dir);

    Direction dir() const;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void move(qreal x, qreal y );
    bool hitTest( const QPointF & point );


private:
    const Direction iDirect;
    QRectF dRect;

};

#endif // DRAGINGRECTITEM_H
