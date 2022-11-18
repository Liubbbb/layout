#include "MainEdit.h"
#include "ui_MainEdit.h"
#include <QScreen>

MainEdit::MainEdit(QString layout,QMainWindow *parent)
    : QMainWindow(parent),
      ui(new Ui::MainEdit)
{
    ui->setupUi(this);
    layoutName = layout;
    int width = QGuiApplication::primaryScreen()->geometry().width();
    int height = QGuiApplication::primaryScreen()->geometry().height();
    setMinimumSize(width,height);
    setMaximumSize(width,height);
    this->setWindowTitle("Custom Settings");
//    setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    ui->indexEdit->setPlaceholderText(tr("0~31")); //输入提示信息
    ui->horizonPosEdit->setText("0");
    ui->verticalPosEdit->setText("0");
    ui->widthEdit->setText("100");
    ui->heightEdit->setText("100");
    ui->view->setStyleSheet("padding: 0px; border: 0px;");
    ui->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene = new CustomScene(layoutName);
    scene->uiInit(ui);
    mul = new MultiView;
    this->showFullScreen();  //全屏显示
    SDVoEAPIRESULT result = mul->retriveLayoutInfo(layoutInf,layoutName);
    scene->setSceneRect(0,0,ui->view->width()-1,ui->view->height()-1);
    scene->setBackgroundBrush(Qt::black);
    ui->view->setScene(scene);
    wScale = layoutInf.width*1.0/scene->width();
    hScale = layoutInf.height*1.0/scene->height();
    //先解析布局
    if(result== SDVoEAPISUCESS)
    {
    //加载布局窗口
        drawWindows();
    }
    connect(ui->addWindowBtn,SIGNAL(clicked()),this,SLOT(addWindow()));
    connect(ui->delWindowBtn,SIGNAL(clicked()),this,SLOT(deleteWindow()));
    connect(ui->exitBnt,SIGNAL(clicked()),this,SLOT(maximiScreen()));
    connect(ui->fullBtn,SIGNAL(clicked()),this,SLOT(fullScreen()));

}

MainEdit::~MainEdit()
{

}

void MainEdit::drawWindows()
{
    if(layoutInf.wdObj.size()>0)
    {
        for(int i= 0;i<layoutInf.wdObj.size();i++)
        {
            scene->addWindowInScene(wScale,hScale,layoutInf.wdObj.at(i));  //获取当前layout的窗口并添加到scene上
            closeEdit = true;
        }
    }
    else {
        return;
    }
}

void MainEdit::addWindow()
{
    StWindowInf windowInf;
    windowInf.index = ui->indexEdit->text().toInt();
    windowInf.width = ui->widthEdit->text().toInt();
    windowInf.height = ui->heightEdit->text().toInt();
    windowInf.horizontal_position = ui->horizonPosEdit->text().toInt();
    windowInf.vertical_position = ui->verticalPosEdit->text().toInt();
    windowInf.layoutName = layoutName;
    // 在此检索该布局信息，查询该布局是否有index等于输入值的window,若有则先删除scene上的item再添加
    if(scene->findIndex(layoutInf,windowInf.index))
        scene->deleteSameIndexItem(windowInf.index);
    mul->createWindow(windowInf); //执行创建窗口命令
    scene->addWindowInScene(wScale,hScale,windowInf);
    closeEdit = true;
}



void MainEdit::closeEvent(QCloseEvent *event)
{
    emit closeWindowEdit(closeEdit);
    event->accept();
}

void MainEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape) close();
    else  e->accept();
}

void MainEdit::deleteWindow()
{
    closeEdit = true;
    scene->deleteWindowInScene();
}

void MainEdit::maximiScreen()
{
    this->showMaximized();
}

void MainEdit::fullScreen()
{
    this->showFullScreen();  //隐藏状态栏和菜单栏
}

void MainEdit::on_saveBtn_clicked()
{
    SDVoEAPIRESULT result = scene->saveLayout();
    QMessageBox *message = new QMessageBox();
    if(result == 0) {
        QString str;
        str = "success";
        message->information(this, "result",str);
        return;
    }
    message->information(this, "result",QString::number(result));
}
