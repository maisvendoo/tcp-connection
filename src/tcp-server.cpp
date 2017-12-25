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

#include    "tcp-server.h"

#include    "blok-data-prepare-engine.h"
#include    "display-data-prepare-engine.h"
#include    "videosystem-data-prepare-engine.h"
#include    "add-launcher.h"


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TcpServer::TcpServer(quint16 port)
    : QTcpServer(Q_NULLPTR)
{
    this->port = port;
    is_started = false;

    connect(this, SIGNAL(newConnection()), this, SLOT(clientConnection()));

    connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(onAcceptError(QAbstractSocket::SocketError)));

    display1_DataPrepareEngine = new DisplayDataPrepareEngine();
    display2_DataPrepareEngine = new DisplayDataPrepareEngine();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TcpServer::~TcpServer()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TcpServer::start()
{
    if (!is_started)
    {
        if (listen(QHostAddress::Any, port))
        {
            QString msg = "OK: TCP-сервер запущен на порту " +
                    QString::number(port);

            emit logPrint(msg);

            is_started = true;
        }
        else
        {
            QString msg = "ERROR: сервер не запущен";
            emit logPrint(msg);

            is_started = false;
        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TcpServer::setEs2gData(es2g_data_t &data)
{
    es2g_data_ = data;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
QByteArray TcpServer::getCommandsArray()
{
    commands_from_clients_t commands;
    bool noDisplays = true;
    bool noLauncher = true;

    QMap<QString, client_t*>::iterator it;

    for (it = authorizedClients_.begin(); it != authorizedClients_.end(); ++it)
    {
        client_t* client = it.value();
        if (client->name.startsWith("Display"))
        {
            if (client->clientData.data_size == sizeof(ref_states_cmd_t))
            {
                ref_states_cmd_t* ref = Q_NULLPTR;
                Additional::mega_cast(ref, client->clientData.data);
                commands.from_refs_ptr(ref);
            }
            noDisplays = false;
        }

        if (client->name == "launcher")
        {
            if (client->clientData.data_size == sizeof(launcher_data_t))
            {
                launcher_data_t* lnc = Q_NULLPTR;
                Additional::mega_cast(lnc, client->clientData.data);
                commands.terminateModel = lnc->terminate;
            }
            noLauncher = false;
        }
    }


    if (noDisplays && noLauncher)
    {
        return QByteArray();
    }

    QByteArray resultArr(sizeof(commands_from_clients_t), Qt::Uninitialized);
    memcpy(resultArr.data(), &commands, sizeof(commands_from_clients_t));
    return resultArr;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TcpServer::sendEs2gDataToClient(char *data)
{
    memcpy(data, &es2g_data_, sizeof(es2g_data_t));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
client_t* TcpServer::getClientByName_(QString name)
{
    QMap<int, client_t *>::iterator it;

    for (it = clients.begin(); it != clients.end(); )
    {
        if (it.value()->name == name) // FIXME - сделать поиск по другому QMap
        {
            return clients[it.key()];
        }
        else
        {
            ++it;
        }
    }

    return Q_NULLPTR;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
client_t* TcpServer::getClientBySocket_(QTcpSocket *socket)
{
    QMap<int, client_t *>::iterator it;

    for (it = clients.begin(); it != clients.end(); )
    {
        if (it.value()->socket == socket)
        {
            return clients[it.key()];
        }
        else
        {
            ++it;
        }
    }

    return Q_NULLPTR;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
QString TcpServer::clientNameFromCmd_(client_cmd_t &cmd)
{
    // Создаём буфер нужного размера и копируем данные
    char* buffName = new char[cmd.data_size + 1];

    memset(buffName, 0, cmd.data_size + 1);

    memcpy(buffName, cmd.data, cmd.data_size);

    // Создаём QСтроку на основе буффера
    QString result(buffName);

    // Удаляем буффер
    delete[] buffName;

    return result;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TcpServer::defineClientDataPrepareEngine_(client_t *client)
{
    if (client->name == "BLOK")
    {
        client->setDataPrepareEngine(new BLOKDataPrepareEngine());
    }
    else if (client->name == "Display1")
    {
//        client->setDataPrepareEngine(new DisplayDataPrepareEngine());
        client->setDataPrepareEngine(display1_DataPrepareEngine);
    }
    else if (client->name == "Display2")
    {
//        client->setDataPrepareEngine(new DisplayDataPrepareEngine());
        client->setDataPrepareEngine(display2_DataPrepareEngine);
    }
    else if (client->name == "videosystem")
    {
        client->setDataPrepareEngine(new VSystemDataPrepareEngine());
    }
    else
    {
        return;
    }

    connect(client->dataEngine,
            SIGNAL(requestEs2gDataFromServer(char*)),
            this, SLOT(sendEs2gDataToClient(char*)));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TcpServer::clientConnection()
{
    client_t *client = new client_t();

    client->socket = nextPendingConnection();
    int client_id = client->socket->socketDescriptor();

    clients.insert(client_id, client);

    connect(client->socket, SIGNAL(readyRead()),
            this, SLOT(receive()));

    connect(client->socket, SIGNAL(disconnected()),
            this, SLOT(clientDisconnected()));

    QString msg = "OK: Подключен клиент: id = " + QString::number(client_id);
    emit logPrint(msg);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TcpServer::onAcceptError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)

    emit logPrint(errorString());
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TcpServer::receive()
{
    QTcpSocket *socket = (QTcpSocket *) sender();

    QByteArray buff = socket->readAll();

    client_cmd_t cmd;

    int size = buff.size();

    if ( size == sizeof(client_cmd_t))
    {
        memcpy(&cmd, buff.data(), sizeof(client_cmd_t));

        parseCommand(socket, cmd);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TcpServer::clientDisconnected()
{
    QTcpSocket* socket = (QTcpSocket*) sender();

    QMap<int, client_t*>::iterator it;


//    for (it = clients.begin(); it != clients.end(); )
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        client_t* client = it.value();
//        if (it.value()->socket == socket)
        if (client->socket == socket)
        {
            // WARNING воможно не удаляется экземпляр клиента !!!
//            authorizedClients_.remove(it.value()->name);
            authorizedClients_.remove(client->name);
            clients.remove(it.key());
//            break;
            delete client;
            return;
        }
//        else
//        {
//            ++it;
//        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TcpServer::parseCommand(QTcpSocket *socket, client_cmd_t cmd)
{
    client_t *client = getClientBySocket_(socket);

    if (client == Q_NULLPTR)
        return;



    switch (cmd.id)
    {
        case AUTHORIZATION:
        {
            emit logPrint("Запрос авторизации от клиента");

            client->name = clientNameFromCmd_(cmd);
            client->is_connected = true;
            defineClientDataPrepareEngine_(client);

            authorizedClients_.insert(client->name, client);

            QString msg = "Клиент " + client->name + " авторизован";
            emit logPrint(msg);

            client->sendAuthorized();

            break;
        }

        case GET_BLOK_DATA:
        {
            if (client->name == "BLOK")
                client->send();
            break;
        }

        case GET_DISPLAY1_DATA:
        {
            if (client->name == "Display1")
            {
                client->setClientData(cmd);
                client->send();
            }
            break;
        }

        case GET_DISPLAY2_DATA:
        {
            if (client->name == "Display2")
            {
                client->setClientData(cmd);
                client->send();
            }
            break;
        }

        case GET_VIDEOSYSTEM_DATA:
        {
            if (client->name == "videosystem")
            {
                client->send();
            }
            break;
        }

        case LAUNCHER_COMMAND:
        {
            if (client->name == "launcher")
            {
                client->setClientData(cmd);
                // Задел на будущие возможности launcher'а
            }
        }

        default:
            break;

    }

}

