// 30/11/2017
#ifndef CLIENT_DELEGATES_H
#define CLIENT_DELEGATES_H

#include <QString>

class QTcpSocket;
class AbstractDataEngine;

#if defined(TCPCONNECTION_LIB)
    #define DELEGATE_EX Q_DECL_EXPORT
#else
    #define DELEGATE_EX Q_DECL_IMPORT
#endif


class DELEGATE_EX AbstractClientDelegate
{
public:
    ///
    AbstractClientDelegate();
    ///
    virtual ~AbstractClientDelegate();

    ///
    QString getName() const;

    ///
    virtual void setName(QString name);

    ///
    virtual void setSocket(QTcpSocket* sock);

    ///
    virtual qintptr getSocketDescriptor() const;

    ///
    virtual bool checkSocket(QTcpSocket* sock) const;

    ///
    void setDataEngine(AbstractDataEngine* engine);

    ///
    void setInputBuffer(QByteArray buf); // FIXME - возможно заменить на следующий

    ///
    virtual void storeInputData() = 0; // на этот, так как сокет у клиента и он может из него читать

    ///
    virtual void setOutputBuffer(QByteArray buf) = 0;

    ///
    QByteArray getInputBuffer() const;

    ///
    virtual void sendAuthorized() = 0;

    ///
    virtual void sendDataToTcpClient() = 0;


protected:
    //
    QString name_;
    //
    QTcpSocket* socket_;
    //
    AbstractDataEngine* engine_;

};



class DELEGATE_EX DummyDelegate Q_DECL_FINAL : public AbstractClientDelegate
{
public:
    ///
    DummyDelegate();
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
    void sendAuthorized() Q_DECL_OVERRIDE;

    ///
    void sendDataToTcpClient() Q_DECL_OVERRIDE;

};



class DELEGATE_EX ClientDelegate Q_DECL_FINAL : public AbstractClientDelegate
{
public:
    ///
    ClientDelegate();
    ///
    ~ClientDelegate();

    ///
    void storeInputData() Q_DECL_OVERRIDE;

    ///
    void setOutputBuffer(QByteArray buf) Q_DECL_OVERRIDE;

    ///
    void sendAuthorized() Q_DECL_OVERRIDE;

    ///
    void sendDataToTcpClient() Q_DECL_OVERRIDE;

};

#endif // CLIENT_DELEGATES_H
