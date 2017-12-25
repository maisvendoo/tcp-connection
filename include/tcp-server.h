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
//    void start_old(); // add port here
    void start(quint16 port);

    ///
//    void setEs2gData(es2g_data_t &data); // delete

    ///
//    QByteArray getCommandsArray(); // delete

    ///
    void setEngineDefiner(AbstractEngineDefiner *definer);

    ///
    AbstractClientDelegate* getClient(QString clientName);


signals:

    /// Печать данных в лог
    void logPrint(ATcp::ServerLogs, QString msg = ""); // delete modify?

    /// delete
    //void sendRefStatesToEs2g(const char* refStates); // delete


public slots:

    ///
//    void sendEs2gDataToClient(char* data); // delete


private:
    //
    AbstractEngineDefiner* engineDefiner_;

    /// Порт, на котором будет слушать сервер
//    quint16 port; // delete - можно запросить у базового класса

    /// Статус сервера
//    bool    is_started; // delete - можно запросить у базового класса

    /// Список подключенных клиентов
//    QMap<int, client_t *> clients_old; // delete
    QMap<qintptr, AbstractClientDelegate*> clients_;

    /// Список авторизованных клиентов
//    QMap<QString, client_t*> authorizedClients_old; // delete
    QMap<QString, AbstractClientDelegate*> authorizedClients_;

    //
    DummyDelegate* dummyClient_;

    ///
    void authorizeClient_(AbstractClientDelegate* clnt, QByteArray name);

    ///
//    client_t* getClientByName_(QString name); // delete
//    AbstractClientDelegate* getClientByName_(QString name);

    ///
//    client_t* getClientBySocket_(QTcpSocket* socket); // delete
//    AbstractClientDelegate* getClientBySocket_(QTcpSocket* socket);

    ///
//    QString clientNameFromCmd_(client_cmd_t &cmd); // delete
//    QString clientNameFromCmd_(tcp_cmd_t &cmd);

    ///
    //void defineClientDataPrepareEngine_(client_t* client);


private slots:

    /// Прием соединения клиентов
//    void clientConnection_old();
    void clientConnection_();

    /// Обработка ошибок соединений
    void onAcceptError(QAbstractSocket::SocketError error);

    /// Прием данных от клиентов
    //void receive_old();
    void receive_();

    /// Обработка отключения клиентов
//    void clientDisconnected_old();
    void clientDisconnected_();

    /// Обработка команд клиентов
    //void parseCommand(QTcpSocket *socket, client_cmd_t cmd); // можно сделать все в receive

};



/*!
 * \struct
 * \brief Информация о подключенном клиенте
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
/*struct client_t
{
    QString         name;           ///< Имя клиента
    QTcpSocket      *socket;        ///< Сокет, открытый клиентом для обмена
    bool            is_connected;   ///< Статус подключения
    AbstractDataPrepareEngine* dataEngine; ///< Механизм подготовки данных
    client_cmd_t    clientData;     ///< Данные присланные клиентом

    client_t()
        : socket(Q_NULLPTR)
        , is_connected(false)
//        , dataEngine(new NullDataPrepareEngine())
        , clientData(client_cmd_t())
    {
        name.clear();
    }

    ~client_t()
    {
        // deleteDataEngine - FIXME (когда избавлюсь от глобальных механизмов подготовки данных в сервере)
    }

    void setDataPrepareEngine(AbstractDataPrepareEngine* engine)
    {
        delete this->dataEngine;
        this->dataEngine = engine;
    }

    void setClientData(client_cmd_t data)
    {
        clientData = data;
        dataEngine->setDataFromClient(clientData);
    }

    void sendAuthorized()
    {
        char authorized[] = "authorized";
        socket->write(static_cast<const char*>(static_cast<void*>(&authorized)),
                      strlen(authorized) + 1);
        socket->flush();
    }

    void send()
    {
        if (socket->isOpen())
        {
            socket->write(dataEngine->getPreparedData());
            socket->flush();
        }
    }
};*/

#endif // TCPSERVER_H
