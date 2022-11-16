#ifndef MULTIVIEW_H
#define MULTIVIEW_H
#include "publicdefine.h"
#include <QMap>
class MultiView
{
public:

    MultiView();

    //创建布局
    SDVoEAPIRESULT createLayout(QString layoutName,int nWidth,int nHeight);
    //检索某个布局详细信息
    SDVoEAPIRESULT retriveLayoutInfo(StLayoutInf &layoutInf, QString layoutName);
    //检索所有布局名称
    SDVoEAPIRESULT retriveLayoutAll(QJsonDocument &doc);
    //删除布局
    SDVoEAPIRESULT deleteLayout(QString layoutName);

    //创建表面
    SDVoEAPIRESULT createSurface(StSurfaceInf surfanceInf);
    //检索表面
    SDVoEAPIRESULT retriveSurfaceInfo(StSurfaceInf &surfanceInf);
    //删除表面
    SDVoEAPIRESULT deleteSurface(QString layoutName, int nSurfaceIndex);

    //创建窗口
    SDVoEAPIRESULT createWindow(StWindowInf windowInf);
    //检索窗口
    SDVoEAPIRESULT retriveWindowInfo(StWindowInf &windowInf);
    //删除窗口
    SDVoEAPIRESULT deleteWindow(QString layoutName, int nWindowIndex);

    //set multiview
    SDVoEAPIRESULT setMultiview(StMultiviewCommand &setMultiview);

    QNetworkRequest setNetworkRequest(EmDeviceGroups emDeviceGroupType,QString strDeviceId="");

    //解析布局列表
    QList<QString> parseLayoutListJson(QJsonDocument doc);
    //解析布局
    void parseLayoutInfoJson(StLayoutInf &layoutInf,QJsonDocument jsonDocument);
    //解析表面
    void parseSurfaceInfoJson(StSurfaceInf surfanceInf,QJsonDocument doc);
    //解析窗口
    void parseWindowsInfoJson(StWindowInf &windowInf,QJsonDocument doc);

    QList<QString> lyNameList; //存储所有布局的名称


private:
    PGMESSAGE pgmessage;





};

#endif // MULTIVIEW_H
