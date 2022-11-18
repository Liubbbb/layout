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
    MainEdit(QString layout,QMainWindow *parent = nullptr);
    ~MainEdit();
    void drawWindows();

    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *e);
    bool closeEdit = false;

private:

    Ui::MainEdit *ui;
    CustomScene *scene;
    MultiView *mul;
    QString layoutName;  //存储布局名称
    StLayoutInf layoutInf;  //存储解析后的布局信息
    qreal wScale;
    qreal hScale;

signals:
    void closeWindowEdit(bool);
    
private slots:
    void addWindow();
    void deleteWindow();
    void maximiScreen();
    void fullScreen();
    void on_saveBtn_clicked();
};
#endif // MAINWINDOW_H
