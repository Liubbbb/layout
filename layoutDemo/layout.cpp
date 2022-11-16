#include "layout.h"
#include "QDebug"

layout::layout()
{
    this->setWindowTitle("Layout");
    this->setGeometry(400,500,1500,200);
    this->setMaximumHeight(200);
    this->setMinimumHeight(200);

    createScrollArea();
    updateLayout();

    QHBoxLayout* m_layout = new QHBoxLayout(this);//设置主窗体布局
    m_layout->addWidget(layoutScrollArea);
    layoutScrollArea->setWidgetResizable(true);



}

void layout::createScrollArea()
{
    //滑动框口
    layoutScrollArea = new QScrollArea(this);
//    layoutScrollArea->setGeometry(0,0,1000,200);
    layoutScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    layoutScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    layoutScrollWidgetContent = new QWidget; //实际界面
    layoutScrollWidgetContent->setMinimumSize(1200,198);
    boxLayout = new QHBoxLayout();  //水平布局

}

void layout::updateLayout()
{
    row=0;
    layoutBtnGroup = new QButtonGroup();
    layoutBtnGroup->setExclusive(false);
    connect(layoutBtnGroup, SIGNAL(buttonClicked(int)),this, SLOT(buttonGroupClicked(int)));
    QJsonDocument doc;
    //检索布局名称，并为每个布局检索窗口信息
    if(mul.retriveLayoutAll(doc) == SDVoEAPISUCESS)
    {
        if(mul.lyNameList.size()>0)
        {
            for(int i =0;i<mul.lyNameList.size();i++)
            {
                StLayoutInf layoutInf; //存储对应布局的详细信息
                if(mul.retriveLayoutInfo(layoutInf,mul.lyNameList.at(i))== SDVoEAPISUCESS)
                {
                    showLayout(layoutInf); //窗口按钮
                }
                row++;
            }
        }
        else {
            QLabel *textLabel = new QLabel("Don't find any layout！");
            QFont font("Microsoft YaHei", 10, 75);
            textLabel->setFont(font);
            textLabel->setFixedSize(200,20);
            boxLayout->addWidget(textLabel);
        }

        QPushButton *addBtn = new QPushButton("Add Layout");
        connect(addBtn, SIGNAL(clicked()),this, SLOT(addLayout()));
        addBtn->setFixedSize(90,32);
        boxLayout->addWidget(addBtn,1);
    }else{
        QLabel *noteLabel = new QLabel("Request failed！");
        QFont font("Microsoft YaHei", 10, 75);
        noteLabel->setFont(font);
        noteLabel->setFixedSize(200,20);
        boxLayout->addWidget(noteLabel);
    }
    boxLayout->addStretch();
    boxLayout->setSpacing(50);
    layoutScrollWidgetContent->setLayout(boxLayout);
    layoutScrollWidgetContent->adjustSize();
    layoutScrollArea->setWidget(layoutScrollWidgetContent);
}

//设一个置布局的按钮
void layout::showLayout(StLayoutInf layoutInf)
{
        layoutButton = new QToolButton;
        layoutButton->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(layoutButton,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(createLayoutMenu()));
        layoutBtnGroup->addButton(layoutButton,row);
        layoutButton->setObjectName(layoutInf.layout_name);  //将按钮别名命名为布局名称
        // 对应窗口
        QPixmap pixmap(layoutInf.width, layoutInf.height); //画板
        pixmap.fill(Qt::white); //画笔
        QPainter painter(&pixmap);
        painter.setPen(QPen(Qt::black,20));
        painter.drawRect(0,0,layoutInf.width, layoutInf.height);
        if(layoutInf.wdObj.size()>0)
        {
            for(int i= 0;i<layoutInf.wdObj.size();i++)
            {
                painter.drawRect(0,0,layoutInf.wdObj.at(i).width,layoutInf.wdObj.at(i).height);
            }
        }
        pixmap.scaled(300,300);
        QIcon icon(pixmap);
        layoutButton->setIcon(icon);
        layoutButton->setIconSize(QSize(100, 80));
        layoutButton->setCheckable(true);
        layoutButton->setStyleSheet("QToolButton{border: none}""QToolButton:hover{background:white}""QToolButton:pressed{background:none}""QToolButton:checked{background-color: qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #c4dbff, stop:1 #c4dbff);color:#4D4D4D;border-radius:4px;border:1px solid #B8B6B6;}");
        layoutButton->setText(layoutInf.layout_name);
        layoutButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        boxLayout->addWidget(layoutButton,1);
}


void layout::createLayoutMenu()
{
    layoutMenu = new QMenu(layoutButton);
    QAction *editAction = new QAction(tr("编辑布局"), this);
    connect(editAction,SIGNAL(triggered(bool)),this,SLOT(editLayout()));
    QAction *delAction = new QAction(tr("删除布局"), this);
    connect(delAction,SIGNAL(triggered(bool)),this,SLOT(deleteLayoutBtn()));
    QAction *infAction = new QAction(tr("详细信息"), this);
    connect(infAction,SIGNAL(triggered(bool)),this,SLOT(layoutInfo()));

    layoutMenu->addAction(editAction);
    layoutMenu->addAction(delAction);
    layoutMenu->addAction(infAction);
    layoutMenu->exec(QCursor::pos());  //下拉菜单显示位置

}

void layout::addLayout()
{
    ly = new newlayoutwidget();
    connect(ly,SIGNAL(closeWidget(bool)),this,SLOT(updateAfterClose(bool)));
    ly->show();
}

void layout::editLayout()
{
    w = new MainEdit(selectButton->objectName());
    connect(w,SIGNAL(closeWindowEdit(bool)),this,SLOT(updateAfterClose(bool)));
    w->show();
}

void layout::deleteLayoutBtn()
{
    delDialog = new QDialog();
    delDialog->setWindowTitle("删除布局");
    delDialog->setFixedSize(300,100);
    delDialog->setWindowModality(Qt::ApplicationModal); //模态窗口
    QVBoxLayout *vBoxLayout = new QVBoxLayout();
    QLabel *textLabel = new QLabel();
    textLabel->setText("确定要删除布局吗？");
    vBoxLayout->addWidget(textLabel,Qt::AlignCenter);
    QHBoxLayout *hBox = new QHBoxLayout();
    QPushButton *okBtn = new QPushButton();
    okBtn->setText("确认");
    connect(okBtn,SIGNAL(clicked()),this,SLOT(deleteLayout()));
    QPushButton *concelBtn = new QPushButton();
    concelBtn->setText("取消");
    connect(concelBtn,SIGNAL(clicked()),this,SLOT(cancelDeleteLayout()));
    hBox->addWidget(okBtn);
    hBox->addWidget(concelBtn);
    vBoxLayout->addLayout(hBox);
    delDialog->setLayout(vBoxLayout);
    delDialog->show();
}

void layout::deleteLayout()
{
    if(mul.deleteLayout(selectButton->objectName()) == SDVoEAPISUCESS)
    {
        deleteHboxWidget();
        updateLayout();
    }
    delDialog->close();

}

void layout::cancelDeleteLayout()
{
    delDialog->close();
}

void layout::layoutInfo()
{
    StLayoutInf layoutInf;
    mul.retriveLayoutInfo(layoutInf,selectButton->objectName());
    inf.getLayoutInf(layoutInf);
    inf.show();
}

void layout::updateAfterClose(bool update)
{
    if(update)
    {
        deleteHboxWidget();
        updateLayout();
    }else return;
}



void layout::deleteHboxWidget()
{
    QLayoutItem *child;
     while ((child = boxLayout->takeAt(0)) != nullptr)
     {
            //setParent为NULL，防止删除之后界面不消失
            if(child->widget())
            {
                child->widget()->setParent(nullptr);
                delete child->widget();//释放
            }

            delete child;
     }
     boxLayout->update();
}

void layout::buttonGroupClicked(int id)
{
    // 判断是否为当前选中的按钮，若不是当前选中的按钮则令其不可选
    QList<QAbstractButton *> buttons = layoutBtnGroup->buttons();
    foreach (QAbstractButton *button, buttons) {
        if (layoutBtnGroup->button(id) != button)
            button->setChecked(false);
    }
    selectButton = layoutBtnGroup->button(id); //存储当前选中按钮
}
