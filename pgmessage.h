#ifndef PGMESSAGE_H
#define PGMESSAGE_H


#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpPart>
#include <QHttpMultiPart>
#include<QEventLoop>
#include<QJsonDocument>
#include<QJsonParseError>
#include <QJsonObject>
#include<QJsonArray>
#include<QTimer>


typedef enum
{
    SDVoEAPISUCESS                  =   0,

    // network layer errors [relating to the destination server] (1-99):
    SDVoENetwork_ConnectionRefusedError = 1,
    SDVoENetwork_RemoteHostClosedError,
    SDVoENetwork_HostNotFoundError,
    SDVoENetwork_TimeoutError,
    SDVoENetwork_OperationCanceledError,
    SDVoENetwork_SslHandshakeFailedError,
    SDVoENetwork_TemporaryNetworkFailureError,
    SDVoENetwork_NetworkSessionFailedError,
    SDVoENetwork_BackgroundRequestNotAllowedError,
    SDVoENetwork_TooManyRedirectsError,
    SDVoENetwork_InsecureRedirectError,
    SDVoENetwork_UnknownNetworkError = 99,

    // proxy errors (101-199):
    SDVoENetwork_ProxyConnectionRefusedError = 101,
    SDVoENetwork_ProxyConnectionClosedError,
    SDVoENetwork_ProxyNotFoundError,
    SDVoENetwork_ProxyTimeoutError,
    SDVoENetwork_ProxyAuthenticationRequiredError,
    SDVoENetwork_UnknownProxyError = 199,

    // content errors (201-299):
    SDVoENetwork_ContentAccessDenied = 201,
    SDVoENetwork_ContentOperationNotPermittedError,
    SDVoENetwork_ContentNotFoundError,
    SDVoENetwork_AuthenticationRequiredError,
    SDVoENetwork_ContentReSendError,
    SDVoENetwork_ContentConflictError,
    SDVoENetwork_ContentGoneError,
    SDVoENetwork_UnknownContentError = 299,

    // protocol errors
    SDVoENetwork_ProtocolUnknownError = 301,
    SDVoENetwork_ProtocolInvalidOperationError,
    SDVoENetwork_ProtocolFailure = 399,

    // Server side errors (401-499)
    SDVoENetwork_InternalServerError = 401,
    SDVoENetwork_OperationNotImplementedError,
    SDVoENetwork_ServiceUnavailableError,
    SDVoENetwork_UnknownServerError = 499,

    /*APIError*/
    SDVoEError_ILLEGAL_ARGUMENT         = 501,          //Occurs when the arguments entered are invalid
    SDVoEError_INVALID_STATE        ,                   //This occurs if the destination device is disconnected.
    SDVoEError_CONFIGURATION        ,                   //If this feature has not been enabled in the API server configuration
    SDVoEError_PROTOCOL_NOT_FOUND   ,                   // This message is provided when the command is not supported by the API
    SDVoEError_DEVICE_TYPE          ,                   // The firmware does not support this feature
    SDVoEError_DEVICE_DISCONNECTED  ,
    SDVoEError_UNKOWN,
    /*jsonError*/
    SDVoEError_WrongJson,
    /*服务器问题*/
    SDVoEError_ServerFailed
}SDVoEAPIRESULT;

typedef enum
{
    ALL_DEVICE,
    ALL_DEVICE_TX,
    ALL_DEVICE_RX,
    SINGLE_DEVICE
}EmDeviceGroups;

class PGMESSAGE: public QObject
{
    Q_OBJECT
public:
    PGMESSAGE();
    ~PGMESSAGE();
    SDVoEAPIRESULT getAPI(const QString ApiStr, QJsonDocument *JDoc);
    SDVoEAPIRESULT postAPI(const QNetworkRequest& request, const QJsonObject& postJson, QJsonDocument *JDoc);
    SDVoEAPIRESULT receiveReply(QNetworkReply *reply, QJsonDocument *JDoc);
    QNetworkRequest setNetworkRequest(EmDeviceGroups emDeviceGroupType, QString strDeviceId);
    SDVoEAPIRESULT  deleteAPI(const QString ApiStr, QJsonDocument *JDoc);

private:
    QString QPort="8080";
    QString myUrl=QString("http://127.0.0.1:%1/api/").arg(QPort);
    QNetworkAccessManager *manager;

};



#endif // PGMESSAGE_H

