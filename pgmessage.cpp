#include "pgmessage.h"

PGMESSAGE::PGMESSAGE()
{
    manager = new QNetworkAccessManager(this);
}

PGMESSAGE::~PGMESSAGE()
{
    delete manager;
}

SDVoEAPIRESULT PGMESSAGE::getAPI(const QString ApiStr, QJsonDocument *JDoc)
{
    //构建请求对象
    QNetworkRequest request;
    request.setUrl(QUrl(myUrl+ApiStr));
    //设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    //超时处理，可以使用定时器调用abort、close来终止当前的请求
    QNetworkReply *reply=manager->get(request);
    if(reply->isRunning()){
        QTimer *timer=new QTimer(reply);//对象树关联释放，也可以在finish进行释放
        timer->setSingleShot(true);
        //超时就结束
        connect(timer,&QTimer::timeout,reply,&QNetworkReply::abort);
        //结束就关定时器
        connect(reply,&QNetworkReply::finished,timer,&QTimer::stop);
        //定时
        timer->start(5000);
        QEventLoop eventLoop;
        connect(reply,&QNetworkReply::finished,&eventLoop, &QEventLoop::quit);
        connect(timer,&QTimer::timeout, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();
    }
    return receiveReply(reply,JDoc);
}


QNetworkRequest PGMESSAGE::setNetworkRequest(EmDeviceGroups emDeviceGroupType, QString strDeviceId="")
{
    QNetworkRequest request;
    switch (emDeviceGroupType) {
    case ALL_DEVICE:
       request.setUrl(QUrl("http://127.0.0.1:8080/api/device/ALL"));
        break;
    case ALL_DEVICE_TX:
       request.setUrl(QUrl("http://127.0.0.1:8080/api/device/ALL_TX"));
        break;
    case ALL_DEVICE_RX:
       request.setUrl(QUrl("http://127.0.0.1:8080/api/device/ALL_RX"));
        break;
    case SINGLE_DEVICE:
       request.setUrl(QUrl(QString("http://127.0.0.1:8080/api/device/%1").arg(strDeviceId)));
        break;
    default:
        break;
    }

    //设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    return request;
}

SDVoEAPIRESULT PGMESSAGE::deleteAPI(const QString ApiStr, QJsonDocument *JDoc)
{
    //构建请求对象
        QNetworkRequest request;
        request.setUrl(QUrl(myUrl+ApiStr));

        //超时处理，可以使用定时器调用abort、close来终止当前的请求
        QNetworkReply *reply=manager->deleteResource(request);
        if(reply->isRunning()){
            QTimer *timer=new QTimer(reply);//对象树关联释放，也可以在finish进行释放
            timer->setSingleShot(true);
            //超时就结束
            connect(timer,&QTimer::timeout,reply,&QNetworkReply::abort);
            //结束就关定时器
            connect(reply,&QNetworkReply::finished,timer,&QTimer::stop);
            //定时
            timer->start(5000);
            QEventLoop eventLoop;
            connect(reply,&QNetworkReply::finished,&eventLoop, &QEventLoop::quit);
            connect(timer,&QTimer::timeout, &eventLoop, &QEventLoop::quit);
            eventLoop.exec();
        }
        return receiveReply(reply,JDoc);
}


SDVoEAPIRESULT PGMESSAGE::postAPI(const QNetworkRequest& request, const QJsonObject& postJson, QJsonDocument *JDoc)
{
    QNetworkReply *reply = manager->post(request,QJsonDocument(postJson).toJson());
    if(reply->isRunning()){
        QTimer *timer=new QTimer(reply);//对象树关联释放，也可以在finish进行释放
        timer->setSingleShot(true);
        //超时就结束
        connect(timer,&QTimer::timeout,reply,&QNetworkReply::abort);
        //结束就关定时器
        connect(reply,&QNetworkReply::finished,timer,&QTimer::stop);
        //定时
        timer->start(5000);
        QEventLoop eventLoop;
        connect(reply,&QNetworkReply::finished,&eventLoop, &QEventLoop::quit);
        connect(timer,&QTimer::timeout, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();
    }
    return receiveReply(reply,JDoc);
}

SDVoEAPIRESULT PGMESSAGE::receiveReply(QNetworkReply *reply, QJsonDocument *JDoc)
{
    SDVoEAPIRESULT Result;
    //路径
    qDebug()<<"url: "<<reply->url().toString();
    //状态码
    const int status_code=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<"status code:"<<status_code;
    //主动调用QNetworkReply的close或者abort也会触发finished，这时状态码为0,访问失败状态码也会为0
    if(status_code==0){
        return SDVoENetwork_TimeoutError;
    }
    //检查是否存在networkError
    QNetworkReply::NetworkError nwerro = reply->error();
    if(nwerro!=QNetworkReply::NoError)
        return SDVoEAPIRESULT(nwerro);

    const QByteArray reply_data=reply->readAll();
    //解析json
    QJsonParseError json_error;
    QJsonDocument document=QJsonDocument::fromJson(reply_data, &json_error);
    *JDoc = document;
    if (!(json_error.error==QJsonParseError::NoError) || !document.isObject())
        return SDVoEError_WrongJson;    //json格式异常

    const QJsonObject obj=document.object();
    QJsonValue jsError = obj.value("error");
    if(!jsError.isNull())
        return SDVoEError_ServerFailed; //服务器处理API异常

    QJsonValue valResult=obj.value("result");
    if(valResult.isNull())      //result 与 error都为空时，返回success
        return SDVoEAPISUCESS;
    //解析result中的error
    QJsonObject objResult=valResult.toObject();
    QJsonArray Arrerrors=objResult.value("error").toArray();
    if(!(Arrerrors.size()==0))
    {
        QJsonObject deviceError = Arrerrors.at(0).toObject();
        QString ErroReason = deviceError.value("reason").toString();
        if(ErroReason=="INVALID_STATE")
            Result = SDVoEError_INVALID_STATE;
        else
            Result=SDVoEError_UNKOWN;
//        switch (ErroReason) {
//        case "INVALID_STATE":  Result = SDVoEError_INVALID_STATE; break;
//        default:               Result=SDVoEError_UNKOWN;          break;
//        }
    }
    else Result=SDVoEAPISUCESS;
    reply->deleteLater();
    return Result;
}





