#ifndef LAYOUT_H
#define LAYOUT_H
#include <QScrollArea>
#include "QLabel"
#include <QGridLayout>
#include <QPainter>
#include <QToolButton>
#include <QIcon>
#include <QButtonGroup>
#include <QPushButton>
#include <QBoxLayout>
#include <QMenu>
#include <QJsonDocument>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QDialog>
#include "newlayoutwidget.h"
#include "MainEdit.h"
#include "layoutinf.h"

class layout: public QWidget
{
    Q_OBJECT
public:
    layout();
    //创建滑动窗口
    void createScrollArea();
    //搜索所有布局
    void updateLayout();
    //布局按钮和图标
    void showLayout(StLayoutInf layoutInf);
    //清除HBoxlayout中的widget
    void deleteHboxWidget();

protected:
    int row;
    MultiView mul;
    LayoutInf inf;
    QScrollArea *layoutScrollArea;
    QWidget * layoutScrollWidgetContent;
    QToolButton * layoutButton;
    QHBoxLayout *boxLayout;
    QGridLayout * btnLayout;
    QButtonGroup * layoutBtnGroup;  //存储按钮对象及ID
    QMenu *layoutMenu;
    QAbstractButton *selectButton;  //当前被选中按钮
    QDialog *delDialog;
    newlayoutwidget *ly;
    MainEdit *w;


//    QList<QToolButton *> layoutToolBtnList;  //存储按钮对象


signals:
    void sendLayoutInfo(StLayoutInf);
    void updateSelectBtn(QAbstractButton *);

private slots:
    void buttonGroupClicked(QAbstractButton *);
    // 右键菜单
    void createLayoutMenu();
//    //添加新布局
    void addLayout();
    //编辑布局
    void editLayout();
    //删除布局按钮
    void deleteLayoutBtn();
    //确认删除布局
    void deleteLayout();
    void cancelDeleteLayout();
    //检索布局详细信息
    void layoutInfo();
    void updateAfterClose(bool update);
    void applayLayout();
    void clickRightButton();






};

#endif // LAYOUT_H
