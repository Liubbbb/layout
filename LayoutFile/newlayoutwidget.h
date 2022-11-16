#ifndef NEWLAYOUTWIDGET_H
#define NEWLAYOUTWIDGET_H

#include <QWidget>


namespace Ui {
class newlayoutwidget;
}

class newlayoutwidget : public QWidget
{
    Q_OBJECT

public:
    explicit newlayoutwidget(QWidget *parent = nullptr);
    ~newlayoutwidget();
    void closeEvent(QCloseEvent *event);
    bool updateNew = false;

signals:
    void closeWidget(bool update);

private slots:
    void on_closeBtn_clicked();
    void on_addBtn_clicked();

private:
    Ui::newlayoutwidget *ui;

};

#endif // NEWLAYOUTWIDGET_H
