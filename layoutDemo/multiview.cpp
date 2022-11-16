#include "multiview.h"

MultiView::MultiView()
{

}

//创建布局,返回状态码
SDVoEAPIRESULT MultiView::createLayout(QString strLayoutName,int nWidth,int nHeight)
{
    QNetworkRequest request;
    const QString apiStr = QString("http://127.0.0.1:8080/api/multiview/layout/%1").arg(strLayoutName);
    request.setUrl(QUrl(apiStr));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("op","create");
    obj.insert("width",nWidth);
    obj.insert("height",nHeight);
    SDVoEAPIRESULT result = pgmessage.postAPI(request,obj,&doc);
    qDebug()<<"crate:"<<result;
    return result;
}

//检索某个布局详细信息
SDVoEAPIRESULT MultiView::retriveLayoutInfo(StLayoutInf &layoutInf,QString strLayoutName)
{

    QJsonDocument doc;
    const QString str = QString("multiview/layout/%1").arg(strLayoutName);
    SDVoEAPIRESULT sdResult = pgmessage.getAPI(str, &doc);
    //检索信息
    if(sdResult == SDVoEAPISUCESS)
    {
        parseLayoutInfoJson(layoutInf,doc);
    }
    return sdResult;
}

//检索所有布局名称
SDVoEAPIRESULT MultiView::retriveLayoutAll(QJsonDocument &doc)
{
    const QString str = QString("multiview/layout");
    SDVoEAPIRESULT result = pgmessage.getAPI(str, &doc);
    if(result == SDVoEAPISUCESS)
    {
        qDebug()<<"result="<<result;
        lyNameList = parseLayoutListJson(doc);
        if(lyNameList.size()>0)
        {
            for(int i =0;i<lyNameList.size();i++)
            {
                qDebug()<<lyNameList.at(i);
            }
        }
        else {
            qDebug()<<"未创建布局！";
        }
    }
    return result;
}

//删除布局
SDVoEAPIRESULT MultiView::deleteLayout(QString layout_name)
{
    QJsonDocument JDoc;
    QString str = QString("multiview/layout/%1").arg(layout_name);
    SDVoEAPIRESULT result = pgmessage.deleteAPI(str,&JDoc);
    return result;
}

//创建表面,给结构体变量赋值后创建
SDVoEAPIRESULT MultiView::createSurface(StSurfaceInf surfanceInf)
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString("http://127.0.0.1:8080/api/multiview/layout/%1/surface/%2").arg(surfanceInf.layoutName).arg(surfanceInf.index)));
    //设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QJsonDocument doc;  //存储返回值
    QJsonObject obj;
    obj.insert("op","create");
    obj.insert("vertical_position",surfanceInf.vertical_position);
    obj.insert("horizontal_position",surfanceInf.horizontal_position);
    obj.insert("width",surfanceInf.width);
    obj.insert("height",surfanceInf.height);

    SDVoEAPIRESULT sdResult = pgmessage.postAPI(request, obj, &doc);
    return sdResult;

}


//检索表面
SDVoEAPIRESULT MultiView::retriveSurfaceInfo(StSurfaceInf &surfanceInf)
{
     QNetworkRequest request;
     QJsonDocument doc;
     QString str = QString("multiview/layout/%1/surface/%2").arg(surfanceInf.layoutName).arg(surfanceInf.index);
     SDVoEAPIRESULT sdResult = pgmessage.getAPI(str,&doc);
     if(sdResult == SDVoEAPISUCESS)
     {
        parseSurfaceInfoJson(surfanceInf,doc);
     }
     return sdResult;
}


//删除表面
SDVoEAPIRESULT MultiView::deleteSurface(QString layoutName, int nSurfsceIndex)
{
    QJsonDocument JDoc;
    QString str = QString("multiview/layout/%1/surface/%2").arg(layoutName).arg(nSurfsceIndex);
    SDVoEAPIRESULT result = pgmessage.deleteAPI(str,&JDoc);
    return result;
}


//创建窗口
SDVoEAPIRESULT MultiView::createWindow(StWindowInf windowInf)
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString("http://127.0.0.1:8080/api/multiview/layout/%1/window/%2").arg(windowInf.layoutName).arg(windowInf.index)));
    //设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QJsonObject obj;
    obj.insert("op","create");
    obj.insert("horizontal_position",windowInf.horizontal_position);
    obj.insert("vertical_position",windowInf.vertical_position);
    obj.insert("width",windowInf.width);
    obj.insert("height",windowInf.height);

    //根据状态位插入被选择的参数
    for (int i=0;i<windowInf.EM_TERGET_SURFANCE+1;i++) {
        if(windowInf.state_bit[i]){
            obj.insert(windowInf.strKey.at(i),windowInf.selectContent[i]);
        }
    }
    QJsonDocument doc;
    SDVoEAPIRESULT sdResult = pgmessage.postAPI(request, obj, &doc);
    return sdResult;

}


//检索窗口
SDVoEAPIRESULT MultiView::retriveWindowInfo(StWindowInf &windowInf)
{
    QNetworkRequest request;
    QJsonDocument doc;
    QString str = QString("multiview/layout/%1/window/%2").arg(windowInf.layoutName).arg(windowInf.index);
    SDVoEAPIRESULT sdResult = pgmessage.getAPI(str,&doc);
    if(sdResult == SDVoEAPISUCESS)
    {
        parseWindowsInfoJson(windowInf,doc);
    }
    return sdResult;
}


//删除窗口
SDVoEAPIRESULT MultiView::deleteWindow(QString layoutName, int nWindowIndex)
{
    QJsonDocument JDoc;
    QString str = QString("multiview/layout/%1/window/%2").arg(layoutName).arg(nWindowIndex);
    SDVoEAPIRESULT result = pgmessage.deleteAPI(str,&JDoc);
    return result;
}

//设置multiview
SDVoEAPIRESULT MultiView::setMultiview(StMultiviewCommand &setMultiview)
{
    QNetworkRequest request = pgmessage.setNetworkRequest(setMultiview.DeviceGroups,setMultiview.strDeviceId);
    QJsonObject obj;
    obj.insert("op","set:multiview");
    obj.insert("layout",setMultiview.layout);

    //插入video
    QJsonObject objVideo;
    for(int i=0;i<setMultiview.stVideoFormatArguments.EM_BEZEL_VERT-5;i++)
    {
        if(setMultiview.stVideoFormatArguments.blSetStatus[i])
        {
            if(i<setMultiview.stVideoFormatArguments.EM_HSYNC_FRONT_PORCH)
                objVideo.insert(setMultiview.stVideoFormatArguments.strKey.at(i),setMultiview.stVideoFormatArguments.strVideoArguments[i]);
            else
                objVideo.insert(setMultiview.stVideoFormatArguments.strKey.at(i),setMultiview.stVideoFormatArguments.nVideoArguments[i-setMultiview.stVideoFormatArguments.EM_HSYNC_FRONT_PORCH]);
        }
    }

    obj.insert("video",objVideo);

    //插入subscriptions,整体可选
    if(setMultiview.subscriptions_state)
    {
        QJsonArray arraySup;

        for(int i = 0;i<setMultiview.length_subscriptions;i++)
        {
            QJsonObject objSub;

            objSub.insert("source_device",setMultiview.source_device.at(i));
            objSub.insert("stream_index",setMultiview.stream_index[i]);
            objSub.insert("subscription_index",setMultiview.subscriptions_index[i]);

            arraySup.append(objSub);
        }

        obj.insert("subscriptions",arraySup);

    }
    QJsonDocument doc;
    SDVoEAPIRESULT sdResult = pgmessage.postAPI(request, obj, &doc);
    return sdResult;

}


//解析布局列表
QList<QString> MultiView::parseLayoutListJson(QJsonDocument doc)
{

    QJsonObject rootObject = doc.object(); //将json格式数据转化为json对象
    QJsonObject resultObject = rootObject.value("result").toObject(); //将对应的值转化为json对象
    QJsonArray layoutArray = resultObject.value("layout").toArray();  //将对应的值转换为数组
    QList<QString> layoutAll;
    if(layoutArray.size() > 0)
    {
        for (int i = 0;i <layoutArray.count();i++)
        {
            QJsonValue layoutArrayObject = layoutArray.at(i);
            QJsonObject layoutArrayObjectValue = layoutArrayObject.toObject();  //将数组的值转换为json对象
            QJsonValue nameValue = layoutArrayObjectValue.value("name");
            layoutAll.append(nameValue.toString());
    //        qDebug()<<layout_inf.layout_list;
        }
    }
    return layoutAll;
}


//解析某个布局详细信息
void MultiView::parseLayoutInfoJson(StLayoutInf &layoutInf,QJsonDocument doc)
{
    QJsonObject rootObject = doc.object();
    QJsonObject resultObject = rootObject.value("result").toObject();
    QJsonObject layout_desoriptionObject = resultObject.value("layout_description").toObject();

    //解析layout自身属性值并存储
    layoutInf.layout_name = layout_desoriptionObject.value("name").toString();
    layoutInf.width = layout_desoriptionObject.value("width").toInt();
    layoutInf.height = layout_desoriptionObject.value("height").toInt();
    layoutInf.read_only = layout_desoriptionObject.value("read_only").toBool();

    //解析surface数组
    QJsonArray layout_surfaceArray = layout_desoriptionObject.value("surfaces").toArray();
    for(int i = 0;i < layout_surfaceArray.count();i++)
    {
        StSurfaceInf sf;
        QJsonObject surfaceObject =  layout_surfaceArray.at(i).toObject();

        sf.index = surfaceObject.value("index").toInt();
        sf.horizontal_position = surfaceObject.value("horizontal_position").toInt();
        sf.vertical_position = surfaceObject.value("vertical_position").toInt();
        sf.width = surfaceObject.value("width").toInt();
        sf.height = surfaceObject.value("height").toInt();

//        qDebug()<<"表面测试:"<<sf.index<<sf.horizontal_position<<sf.vertical_position<<sf.width<<sf.height;

        layoutInf.sfObj.append(sf);
    }
   //解析windows数组
   QJsonArray windowsArray = layout_desoriptionObject.value("windows").toArray();

//   qDebug()<<"窗口数组长度"<<windowsArray.size();  //窗口可能为空

   if(windowsArray.size()>0)
   {
       for(int j = 0;j < windowsArray.count();j++)
       {
           StWindowInf wd;
           QJsonObject windowsObject = windowsArray.at(j).toObject();

           wd.index  =windowsObject.value("index").toInt();
           wd.horizontal_position= windowsObject.value("horizontal_position").toInt();
           wd.vertical_position = windowsObject.value("vertical_position").toInt();
           wd.width = windowsObject.value("width").toInt();
           wd.height = windowsObject.value("height").toInt();
           wd.horizontal_offset = windowsObject.value("horizontal_offset").toInt();
           wd.vertical_offset = windowsObject.value("vertical_offset").toInt();
           wd.content = windowsObject.value("content").toString();
           wd.target_surface = windowsObject.value("target_surface").toInt();

//           qDebug()<<"窗口测试:"<<wd.index<<wd.horizontal_position<<wd.vertical_position;
//           qDebug()<<"测试"<<wd.width<<wd.height<<wd.horizontal_offset<<wd.vertical_offset;
//           qDebug()<<"测试"<<wd.content<<wd.target_surface;

           layoutInf.wdObj.append(wd);
       }
   }

}

//解析表面
void MultiView::parseSurfaceInfoJson(StSurfaceInf surfaceInf,QJsonDocument doc)
{
    QJsonObject rootObj = doc.object();
    QJsonObject resultObj = rootObj.value("result").toObject();
    QJsonObject surfaceObj = resultObj.value("layout_surface").toObject();

    surfaceInf.index = surfaceObj.value("index").toInt();
    surfaceInf.horizontal_position = surfaceObj.value("horizontal_position").toInt();
    surfaceInf.vertical_position = surfaceObj.value("vertical_position").toInt();
    surfaceInf.width = surfaceObj.value("width").toInt();
    surfaceInf.height = surfaceObj.value(("height")).toInt();

}

//解析窗口
void MultiView::parseWindowsInfoJson(StWindowInf &windowInf,QJsonDocument doc)
{
    QJsonObject rootObj = doc.object();
    QJsonObject resultObj = rootObj.value("result").toObject();
    QJsonObject windowObj = resultObj.value("layout_window").toObject();

    windowInf.index = windowObj.value("index").toInt();
    windowInf.width = windowObj.value("width").toInt();
    windowInf.height = windowObj.value("height").toInt();
    windowInf.horizontal_position = windowObj.value("horizontal_position").toInt();
    windowInf.vertical_position = windowObj.value("vertical_position").toInt();
    windowInf.horizontal_offset = windowObj.value("horizontal_offset").toInt();
    windowInf.vertical_offset = windowObj.value("vertical_offset").toInt();
    windowInf.content = windowObj.value("content").toString();
    windowInf.target_surface = windowObj.value("target_surface").toInt();
}

