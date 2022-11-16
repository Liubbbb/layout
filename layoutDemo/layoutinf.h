#ifndef LAYOUTINF_H
#define LAYOUTINF_H

#include <QWidget>
#include "publicdefine.h"

namespace Ui {
class LayoutInf;
}

class LayoutInf : public QWidget
{
    Q_OBJECT

public:
    explicit LayoutInf(QWidget *parent = nullptr);
    ~LayoutInf();
    void getLayoutInf(StLayoutInf layoutInf);

private:
    Ui::LayoutInf *ui;
};

#endif // LAYOUTINF_H
