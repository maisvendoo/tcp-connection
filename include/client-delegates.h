// 30/11/2017
#ifndef CLIENT_DELEGATES_H
#define CLIENT_DELEGATES_H

#include <QObject>
#include <QString>
#include <a-tcp-namespace.h>

class QTcpSocket;
class AbstractDataEngine;

#if defined(TCPCONNECTION_LIB)
    #define DELEGATE_EX Q_DECL_EXPORT
#else
    #define DELEGATE_EX Q_DECL_IMPORT
#endif


class DELEGATE_EX AbstractClientDelegate : public QObject
{
    Q_OBJECT

public:
    ///
    AbstractClientDelegate(QObject* parent = Q_NULLPTR);
    ///
    virtual ~AbstractClientDelegate();

    ///
    QString getName() const;

    ///
    virtual void setName(QString name);

    ///
    virtual void setSocket(QTcpSocket* sock);

    ///
    virtual qintptr getId() const;

    ///
    void setDataEngine(AbstractDataEngine* engine);

    ///
    virtual void storeInputData() = 0;

    ///
    virtual void setOutputBuffer(QByteArray buf) = 0;

    ///
    QByteArray getInputBuffer() const;

    ///
    virtual void sendAuthorizationResponse(ATcp::AuthResponse resp) = 0;

    ///
    virtual void sendDataToTcpClient() = 0;


signals:
    ///
    void dataReceived(QByteArray arr);


protected:
    //
    QString name_;
    //
    qintptr localId_;
    //
    QTcpSocket* socket_;
    //
    AbstractDataEngine* engine_;

};



class DELEGATE_EX DummyDelegate Q_DECL_FINAL : public AbstractClientDelegate
{
    Q_OBJECT

public:
    ///
    DummyDelegate(QObject *parent = Q_NULLPTR);
    ///
    ~DummyDelegate();

    ///
    void setName(QString name) Q_DECL_OVERRIDE;

    ///
    void setSocket(QTcpSocket* sock) Q_DECL_OVERRIDE;

    ///
    void storeInputData() Q_DECL_OVERRIDE;

    ///
    void setOutputBuffer(QByteArray buf) Q_DECL_OVERRIDE;

    ///
    void sendAuthorizationResponse(ATcp::AuthResponse resp) Q_DECL_OVERRIDE;

    ///
    void sendDataToTcpClient() Q_DECL_OVERRIDE;

};



class DELEGATE_EX ClientDelegate Q_DECL_FINAL : public AbstractClientDelegate
{
public:
    ///
    ClientDelegate(QObject* parent = Q_NULLPTR);
    ///
    ~ClientDelegate();

    ///
    void storeInputData() Q_DECL_OVERRIDE;

    ///
    void setOutputBuffer(QByteArray buf) Q_DECL_OVERRIDE;

    ///
//    void sendAuthorized() Q_DECL_OVERRIDE;

    ///
//    void sendDenied(ATcp::ServerLogs errId) Q_DECL_OVERRIDE;

    ///
    void sendAuthorizationResponse(ATcp::AuthResponse resp) Q_DECL_OVERRIDE;

    ///
    void sendDataToTcpClient() Q_DECL_OVERRIDE;

};

#endif // CLIENT_DELEGATES_H
