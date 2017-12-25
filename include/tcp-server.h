//------------------------------------------------------------------------------
//
//      TCP-сервер для связи с клиентскими частями тренажера
//      (с) РГУПС, ВЖД 22/06/2017
//      Разработал: Притыкин Д.Е.
//
//------------------------------------------------------------------------------
/*!
 * \file
 * \brief TCP-сервер для связи с клиентскими частями тренажера
 * \copyright РГУПС, ВЖД
 * \author Притыкин Д.Е.
 * \date 22/06/2017
 */

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include    <QObject>
#include    <QTcpServer>
#include    <QTcpSocket>
#include    <QMap>

#include    "tcp-server-structs.h"

#include    "abstract-data-prepare-engine.h"
#include    "null-data-prepare-engine.h"

class DisplayDataPrepareEngine;


/*!
 * \struct
 * \brief Информация о подключенном клиенте
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
struct client_t
{
    QString         name;           ///< Имя клиента
    QTcpSocket      *socket;        ///< Сокет, открытый клиентом для обмена
    bool            is_connected;   ///< Статус подключения
    AbstractDataPrepareEngine* dataEngine; ///< Механизм подготовки данных
    client_cmd_t    clientData;     ///< Данные присланные клиентом

    client_t()
        : socket(Q_NULLPTR)
        , is_connected(false)
        , dataEngine(new NullDataPrepareEngine())
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
};



/*!
 * \class
 * \brief Реализация логики работы TCP-сервера
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class TcpServer : public QTcpServer
{
    Q_OBJECT

public:

    TcpServer(quint16 port);
    virtual ~TcpServer();

    /// Запуск сервера
    void start();

    ///
    void setEs2gData(es2g_data_t &data);

    ///
    QByteArray getCommandsArray();


signals:

    /// Печать данных в лог
    void logPrint(QString msg);

    /// delete
    void sendRefStatesToEs2g(const char* refStates);


public slots:

    ///
    void sendEs2gDataToClient(char* data);


private:

    /// Порт, на котором будет слушать сервер
    quint16 port;

    /// Статус сервера
    bool    is_started;

    ///
    es2g_data_t es2g_data_;

    ///
    data_from_train_t data_from_train;

    ///
    data_from_traction_drive_t data_from_traction_drive;

    ///
    data_from_panel_t data_from_panel;

    ///
    DisplayDataPrepareEngine* display1_DataPrepareEngine;
    DisplayDataPrepareEngine* display2_DataPrepareEngine;

    /// Список подключенных клиентов
    QMap<int, client_t *> clients;

    /// Список авторизованных клиентов
    QMap<QString, client_t*> authorizedClients_;

    client_t* getClientByName_(QString name);

    ///
    client_t* getClientBySocket_(QTcpSocket* socket);

    ///
    QString clientNameFromCmd_(client_cmd_t &cmd);

    ///
    void defineClientDataPrepareEngine_(client_t* client);


private slots:

    /// Прием соединения клиентов
    void clientConnection();

    /// Обработка ошибок соединений
    void onAcceptError(QAbstractSocket::SocketError error);

    /// Прием данных от клиентов
    void receive();

    /// Обработка отключения клиентов
    void clientDisconnected();

    /// Обработка команд клиентов
    void parseCommand(QTcpSocket *socket, client_cmd_t cmd);

};

#endif // TCPSERVER_H
