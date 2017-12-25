// 29 11 2017

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QMap>

#include "a-tcp-namespace.h"

class QTcpSocket;

class AbstractClientDelegate;
class DummyDelegate;
class AbstractEngineDefiner;


#if defined(TCPCONNECTION_LIB)
    #define TCPSERVER_EXPORT Q_DECL_EXPORT
#else
    #define TCPSERVER_EXPORT Q_DECL_IMPORT
#endif


/*!
 * \class
 * \brief Реализация логики работы TCP-сервера
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class TCPSERVER_EXPORT TcpServer : public QTcpServer
{
    Q_OBJECT

public:

    TcpServer(QObject* parent = Q_NULLPTR);
    virtual ~TcpServer();

    /// Запуск сервера
    void start(quint16 port);

    ///
    void setEngineDefiner(AbstractEngineDefiner *definer);

    ///
    AbstractClientDelegate* getClient(QString clientName);


signals:

    /// Печать данных в лог
    void logPrint(ATcp::ServerLogs, QString msg = "");


private:
    //
    AbstractEngineDefiner* engineDefiner_;

    /// Список подключенных клиентов
    QMap<qintptr, AbstractClientDelegate*> clients_;

    /// Список авторизованных клиентов
    QMap<QString, AbstractClientDelegate*> authorizedClients_;

    //
    DummyDelegate* dummyClient_;

    ///
    void authorizeClient_(AbstractClientDelegate* clnt, QByteArray name);


private slots:

    /// Прием соединения клиентов
    void clientConnection_();

    /// Обработка ошибок соединений
    void onAcceptError(QAbstractSocket::SocketError error);

    /// Прием данных от клиентов
    void receive_();

    /// Обработка отключения клиентов
    void clientDisconnected_();
};

#endif // TCPSERVER_H
