#ifndef MAINEDIT_H
#define MAINEDIT_H

#include "customscene.h"

#include <QMainWindow>
#include <QDesktopWidget>

namespace Ui {
class MainEdit;
}

class MainEdit : public QMainWindow
{
    Q_OBJECT

public:
    MainEdit(QString layout,QWidget *parent = nullptr);
    ~MainEdit();
    void drawWindows();
    bool findIndex(StLayoutInf sLayoutInf,int index);
    void closeEvent(QCloseEvent *event);
    bool closeEdit = false;

private:

    Ui::MainEdit *ui;
    CustomScene *scene;
    MultiView *mul;
    QString layoutName;  //存储布局名称
    StLayoutInf layoutInf;  //存储解析后的布局信息
    qreal wScale;
    qreal hwScale;

signals:
    void closeWindowEdit(bool);

    
private slots:
    void addWindow();
    void deleteWindow();
    void maximiScreen();
    void fullScreen();
    

};
#endif // MAINWINDOW_H
