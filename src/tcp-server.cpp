// 29 11 2017

#include "tcp-server.h"

#include <QTcpSocket>

#include "tcp-structs.h"
#include "abstract-engine-definer.h"
#include "client-delegates.h"


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TcpServer::TcpServer(QObject *parent)
    : QTcpServer(parent)
    , engineDefiner_(new NullDataEngineDefiner())
    , dummyClient_(new DummyDelegate())
{
//    this->port = port;
//    is_started = false;

    connect(this, SIGNAL(newConnection()), this, SLOT(clientConnection_()));

    connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(onAcceptError(QAbstractSocket::SocketError)));
}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TcpServer::~TcpServer()
{
    if (engineDefiner_)
        delete engineDefiner_;

    delete dummyClient_;

    // WARN FIXME !!! HERE Очистить мэпы!
}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//void TcpServer::start_old()
//{
//    if (!is_started)
//    {
//        if (listen(QHostAddress::Any, port))
//        {
//            QString msg = "OK: TCP-сервер запущен на порту " +
//                    QString::number(port);

//            emit logPrint(msg);

//            is_started = true;
//        }
//        else
//        {
//            QString msg = "ERROR: сервер не запущен";
//            emit logPrint(msg);

//            is_started = false;
//        }
//    }
//}




void TcpServer::start(quint16 port)
{
    if (!isListening())
    {
        if (listen(QHostAddress::Any, port))
        {
//            QString msg("OK: TCP-сервер запущен на порту ");
//            msg.append(QString::number(port));
//            emit logPrint(msg);

            emit logPrint(ATcp::OK_SERVER_STARTED, QString::number(port));
        }
        else
        {
//            QString msg("ERROR: сервер не запущен");
//            emit logPrint(msg);

            emit logPrint(ATcp::ER_SERVER_NOT_STARTED, QString::number(port));
        }
    }
}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//void TcpServer::setEs2gData(es2g_data_t &data)
//{
//    es2g_data_ = data;
//}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//QByteArray TcpServer::getCommandsArray()
//{
//    commands_from_clients_t commands;
//    bool noDisplays = true;
//    bool noLauncher = true;

//    QMap<QString, client_t*>::iterator it;

//    for (it = authorizedClients_old.begin(); it != authorizedClients_old.end(); ++it)
//    {
//        client_t* client = it.value();
//        if (client->name.startsWith("Display"))
//        {
//            if (client->clientData.data_size == sizeof(ref_states_cmd_t))
//            {
//                ref_states_cmd_t* ref = Q_NULLPTR;
//                Additional::mega_cast(ref, client->clientData.data);
//                commands.from_refs_ptr(ref);
//            }
//            noDisplays = false;
//        }

//        if (client->name == "launcher")
//        {
//            if (client->clientData.data_size == sizeof(launcher_data_t))
//            {
//                launcher_data_t* lnc = Q_NULLPTR;
//                Additional::mega_cast(lnc, client->clientData.data);
//                commands.terminateModel = lnc->terminate;
//            }
//            noLauncher = false;
//        }
//    }


//    if (noDisplays && noLauncher)
//    {
//        return QByteArray();
//    }

//    QByteArray resultArr(sizeof(commands_from_clients_t), Qt::Uninitialized);
//    memcpy(resultArr.data(), &commands, sizeof(commands_from_clients_t));
//    return resultArr;
//}




void TcpServer::setEngineDefiner(AbstractEngineDefiner *definer)
{
    if (engineDefiner_)
        delete engineDefiner_;

    engineDefiner_ = std::move(definer);
}




void TcpServer::authorizeClient_(AbstractClientDelegate *clnt, QByteArray name)
{
    QString newName(name);
    if (authorizedClients_.contains(newName))
    {
        emit logPrint(ATcp::ER_CLIENT_NAME_DUPLICATE, clnt->getName());
    }
    else
    {
        clnt->setName(newName);
        engineDefiner_->setDataEngine(clnt);
        authorizedClients_.insert(clnt->getName(), clnt);
        clnt->sendAuthorized();

        emit logPrint(ATcp::OK_CLIENT_AUTHORIZED, clnt->getName());
    }
}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//void TcpServer::sendEs2gDataToClient(char *data)
//{
//    memcpy(data, &es2g_data_, sizeof(es2g_data_t));
//}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//client_t* TcpServer::getClientByName_(QString name)
//{
//    QMap<int, client_t *>::iterator it;

//    for (it = clients_old.begin(); it != clients_old.end(); )
//    {
//        if (it.value()->name == name) // FIXME - сделать поиск по другому QMap
//        {
//            return clients_old[it.key()];
//        }
//        else
//        {
//            ++it;
//        }
//    }
//    this->isListening();

//    return Q_NULLPTR;
//}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//client_t* TcpServer::getClientBySocket_(QTcpSocket *socket)
//{
//    QMap<int, client_t *>::iterator it;

//    for (it = clients_old.begin(); it != clients_old.end(); )
//    {
//        if (it.value()->socket == socket)
//        {
//            return clients_old[it.key()];
//        }
//        else
//        {
//            ++it;
//        }
//    }

//    return Q_NULLPTR;
//}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//QString TcpServer::clientNameFromCmd_(client_cmd_t &cmd)
//{
//    // Создаём буфер нужного размера и копируем данные
//    char* buffName = new char[cmd.data_size + 1];

//    memset(buffName, 0, cmd.data_size + 1);

//    memcpy(buffName, cmd.data, cmd.data_size);

//    // Создаём QСтроку на основе буффера
//    QString result(buffName);

//    // Удаляем буффер
//    delete[] buffName;

//    return result;
//}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//void TcpServer::defineClientDataPrepareEngine_(client_t *client)
//{
//    if (client->name == "BLOK")
//    {
//        client->setDataPrepareEngine(new BLOKDataPrepareEngine());
//    }
//    else if (client->name == "Display1")
//    {
////        client->setDataPrepareEngine(new DisplayDataPrepareEngine());
//        client->setDataPrepareEngine(display1_DataPrepareEngine);
//    }
//    else if (client->name == "Display2")
//    {
////        client->setDataPrepareEngine(new DisplayDataPrepareEngine());
//        client->setDataPrepareEngine(display2_DataPrepareEngine);
//    }
//    else if (client->name == "videosystem")
//    {
//        client->setDataPrepareEngine(new VSystemDataPrepareEngine());
//    }
//    else
//    {
//        return;
//    }

//    connect(client->dataEngine,
//            SIGNAL(requestEs2gDataFromServer(char*)),
//            this, SLOT(sendEs2gDataToClient(char*)));
//}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//void TcpServer::clientConnection_old()
//{
//    client_t *client = new client_t();

//    client->socket = nextPendingConnection();
//    int client_id = client->socket->socketDescriptor();

//    clients_old.insert(client_id, client);

//    connect(client->socket, SIGNAL(readyRead()),
//            this, SLOT(receive_old()));

//    connect(client->socket, SIGNAL(disconnected()),
//            this, SLOT(clientDisconnected_old()));

//    QString msg = "OK: Подключен клиент: id = " + QString::number(client_id);
//    emit logPrint(msg);
//}




void TcpServer::clientConnection_()
{
    QTcpSocket * sock = nextPendingConnection();

    ClientDelegate* client = new ClientDelegate();
    client->setSocket(sock);
    qintptr client_id = sock->socketDescriptor();

    clients_.insert(client_id, client);

    connect(sock, SIGNAL(readyRead()),
            this, SLOT(receive_()));

    connect(sock, SIGNAL(disconnected()),
            this, SLOT(clientDisconnected_()));

    emit logPrint(ATcp::OK_NEW_CLIENT_CONNECTED, QString::number(client_id));
}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TcpServer::onAcceptError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);

//    emit logPrint(errorString());

    emit logPrint(ATcp::ER_SERVER_INTERNAL_ERROR, errorString());
}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//void TcpServer::receive_old()
//{
//    QTcpSocket *socket = (QTcpSocket *) sender();

//    QByteArray buff = socket->readAll();

//    client_cmd_t cmd;

//    int size = buff.size();

//    if ( size == sizeof(client_cmd_t))
//    {
//        memcpy(&cmd, buff.data(), sizeof(client_cmd_t));

//        parseCommand(socket, cmd);
//    }
//}




void TcpServer::receive_()
{
    //
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());

    QByteArray arr = sock->readAll();

    //
    size_t infoDataSize = sizeof(ATcp::TcpCommand) + sizeof(size_t);
    if (static_cast<size_t>(arr.size()) < infoDataSize)
        return;

    //
//    ATcp::TcpCommand cmd = ATcp::tcZERO;
//    memcpy(&cmd, arr.data(), sizeof(ATcp::TcpCommand));
    ATcp::TcpCommand cmd = tcp_cmd_t::extractCommand(arr);

    //
//    ptrdiff_t offset = sizeof(ATcp::TcpCommand);

//    size_t data_size = 0;
//    memcpy(&data_size, arr.data() + offset, sizeof(size_t));
    size_t data_size = tcp_cmd_t::extractBufferSize(arr);

    //
//    offset += sizeof(size_t);

//    if (data_size != static_cast<size_t>(arr.size()) - offset)
    if (data_size != static_cast<size_t>(arr.size()) - tcp_cmd_t::INFO_SIZE)
        return;

    //
    quintptr cl_id = sock->socketDescriptor();
    AbstractClientDelegate* client = clients_.value(cl_id, dummyClient_);

    //
    switch (cmd)
    {
    case ATcp::tcZERO:
        return;
        break;

    case ATcp::tcAUTHORIZATION:
        emit logPrint(ATcp::IN_AUTHORIZATION_REQUEST, QString::number(cl_id));
        authorizeClient_(client, arr.right(data_size));
        break;

    case ATcp::tcGET:
        client->sendDataToTcpClient();
        break;

    case ATcp::tcPOST:
        // client->storeInputData();
        client->setInputBuffer(arr.right(data_size));
        break;

    case ATcp::tcPOSTGET:
        // client->storeInputData();
        client->setInputBuffer(arr.right(data_size));
        client->sendDataToTcpClient();
        break;

    default:
        break;
    }
}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//void TcpServer::clientDisconnected_old()
//{
//    QMap<int, client_t*>::iterator it;


//    for (it = clients.begin(); it != clients.end(); )
//    for (it = clients_old.begin(); it != clients_old.end(); ++it)
//    {
//        client_t* client = it.value();
//        if (it.value()->socket == socket)
//        if (client->socket == socket)
//        {
//            authorizedClients_.remove(it.value()->name);
//            authorizedClients_old.remove(client->name);
//            clients_old.remove(it.key());
//            break;
//            delete client;
//            return;
//        }
//        else
//        {
//            ++it;
//        }
//    }
//}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TcpServer::clientDisconnected_()
{
    QTcpSocket* sock = (QTcpSocket*) sender();

    quintptr client_id = sock->socketDescriptor();

    AbstractClientDelegate* client = clients_.value(client_id, Q_NULLPTR);

    if (!client)
        return;

    clients_.remove(client_id);
    authorizedClients_.remove(client->getName());

    delete client;
}




//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//void TcpServer::parseCommand(QTcpSocket *socket, client_cmd_t cmd)
//{
//    client_t *client = getClientBySocket_(socket);

//    if (client == Q_NULLPTR)
//        return;



//    switch (cmd.id)
//    {
//        case AUTHORIZATION:
//        {
//            emit logPrint("Запрос авторизации от клиента");

//            client->name = clientNameFromCmd_(cmd);
//            client->is_connected = true;
//            defineClientDataPrepareEngine_(client);

//            authorizedClients_old.insert(client->name, client);

//            QString msg = "Клиент " + client->name + " авторизован";
//            emit logPrint(msg);

//            client->sendAuthorized();

//            break;
//        }

//        case GET_BLOK_DATA:
//        {
//            if (client->name == "BLOK")
//                client->send();
//            break;
//        }

//        case GET_DISPLAY1_DATA:
//        {
//            if (client->name == "Display1")
//            {
//                client->setClientData(cmd);
//                client->send();
//            }
//            break;
//        }

//        case GET_DISPLAY2_DATA:
//        {
//            if (client->name == "Display2")
//            {
//                client->setClientData(cmd);
//                client->send();
//            }
//            break;
//        }

//        case GET_VIDEOSYSTEM_DATA:
//        {
//            if (client->name == "videosystem")
//            {
//                client->send();
//            }
//            break;
//        }

//        case LAUNCHER_COMMAND:
//        {
//            if (client->name == "launcher")
//            {
//                client->setClientData(cmd);
//                // Задел на будущие возможности launcher'а
//            }
//        }

//        default:
//            break;

//    }

//}

