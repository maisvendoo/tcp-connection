//-----------------------------------------------------------------------------
//
//      Класс TCP сервера для общения с TCP клиентами
//      (c) РГУПС, ВЖД 29/11/2017
//      Разработал: Ковшиков С. В.
//
//-----------------------------------------------------------------------------
/*!
 *  \file
 *  \brief Класс TCP сервера для общения с TCP клиентами
 *  \copyright РГУПС, ВЖД
 *  \author Ковшиков С. В
 *  \date 29/11/2017
 */

#include "tcp-server.h"

#include <QTcpSocket>
#include <QStringBuilder>

#include "tcp-structs.h"
#include "abstract-engine-definer.h"
#include "client-delegates.h"


//------------------------------------------------------------------------------
// Конструктор
//------------------------------------------------------------------------------
TcpServer::TcpServer(QObject *parent)
    : QTcpServer(parent)
    , engineDefiner_(new NullDataEngineDefiner())
    , dummyClient_(new DummyDelegate())
{
    connect(this, SIGNAL(newConnection()), this, SLOT(clientConnection_()));

    connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(onAcceptError(QAbstractSocket::SocketError)));
}




//------------------------------------------------------------------------------
// Деструктор
//------------------------------------------------------------------------------
TcpServer::~TcpServer()
{
    if (engineDefiner_)
        delete engineDefiner_;

    delete dummyClient_;

    QMap<QTcpSocket*, AbstractClientDelegate*>::iterator it;

    for (it = newClients_.begin(); it != newClients_.end(); it++)
    {
        delete it.value();
    }

    newClients_.clear();
    authorizedClients_.clear();
}



//-----------------------------------------------------------------------------
// Запуск сервера
//-----------------------------------------------------------------------------
void TcpServer::start(quint16 port)
{
    if (!isListening())
    {
        if (listen(QHostAddress::Any, port))
        {
            emit logPrint(ATcp::sc_OK_SERVER_STARTED, QString::number(port));
        }
        else
        {
            emit logPrint(ATcp::sc_ER_SERVER_NOT_STARTED,
                          QString::number(port));
        }
    }
}



//-----------------------------------------------------------------------------
// Установить список допустимых имён клиентов
//-----------------------------------------------------------------------------
void TcpServer::setPossibleClients(QStringList names)
{
    possibleClients_ = std::move(names);
}



//-----------------------------------------------------------------------------
// Установить класс определения механизма подготовки данных клиентов
//-----------------------------------------------------------------------------
void TcpServer::setEngineDefiner(AbstractEngineDefiner *definer)
{
    if (engineDefiner_)
        delete engineDefiner_;

    engineDefiner_ = std::move(definer);
}



//-----------------------------------------------------------------------------
// Вернуть клиента по имени
//-----------------------------------------------------------------------------
AbstractClientDelegate *TcpServer::getClient(QString clientName)
{
    return authorizedClients_.value(clientName, dummyClient_);
}



//-----------------------------------------------------------------------------
// Включить/отключить класс делегата-пустышки
//-----------------------------------------------------------------------------
void TcpServer::enableDummy(bool enabled)
{
    if (enabled)
    {
        if (!dummyClient_)
            dummyClient_ = new DummyDelegate();
    }
    else
    {
        if (dummyClient_)
            delete dummyClient_;
        dummyClient_ = Q_NULLPTR;
    }
}



//-----------------------------------------------------------------------------
// Авторизовать клиента с заданным именем
//-----------------------------------------------------------------------------
void TcpServer::authorizeClient_(AbstractClientDelegate *clnt, QByteArray name)
{
    QString newName(name);

    if ( !(possibleClients_.isEmpty() || possibleClients_.contains(name)) )
    {
        clnt->sendAuthorizationResponse(ATcp::ar_UNKNOWN_NAME);

        emit logPrint(ATcp::sc_ER_CLIENT_UNKNOWN_NAME,
                      newName % ":" % QString::number(clnt->getId()));

        return;
    }

    if (authorizedClients_.contains(newName))
    {
        clnt->sendAuthorizationResponse(ATcp::ar_NAME_DUPLICATE);

        emit logPrint(ATcp::sc_ER_CLIENT_NAME_DUPLICATE,
                      newName % ":" % QString::number(clnt->getId()));

        return;
    }

    clnt->setName(newName);
    engineDefiner_->setDataEngine(clnt);
    authorizedClients_.insert(clnt->getName(), clnt);

    clnt->sendAuthorizationResponse(ATcp::ar_AUTHORIZED);

    emit logPrint(ATcp::sc_OK_CLIENT_AUTHORIZED,
                  newName % ":" % QString::number(clnt->getId()));

    emit clientAuthorized(clnt);
}



//-----------------------------------------------------------------------------
// Прием соединения клиентов
//-----------------------------------------------------------------------------
void TcpServer::clientConnection_()
{
    QTcpSocket * sock = nextPendingConnection();

    ClientDelegate* client = new ClientDelegate();
    client->setSocket(sock);

    newClients_.insert(sock, client);

    connect(sock, SIGNAL(readyRead()),
            this, SLOT(receive_()));

    connect(sock, SIGNAL(disconnected()),
            this, SLOT(clientDisconnected_()));

    emit logPrint(ATcp::sc_OK_NEW_CLIENT_CONNECTED,
                  QString::number(sock->socketDescriptor()));
}



//-----------------------------------------------------------------------------
// Прием данных от клиентов
//-----------------------------------------------------------------------------
void TcpServer::receive_()
{
    //
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());

    if (!tcp_cmd_t::validInfoSize(sock))
        return;

    //
    tcp_cmd_t::info_t info = tcp_cmd_t::extractInfo(sock);

    //
    if(!tcp_cmd_t::validBufferSize(info.bufferSize, sock))
        return;

    //
    AbstractClientDelegate* client = newClients_.value(sock, dummyClient_);

    if(!client)
        return;

    //
    switch (info.command)
    {
    //
    case ATcp::tcZERO:
        return;
        break;

    //
    case ATcp::tcAUTHORIZATION:
        emit logPrint(ATcp::sc_IN_AUTHORIZATION_REQUEST,
                      QString::number(sock->socketDescriptor()));
        authorizeClient_(client, sock->readAll());
        break;

    //
    case ATcp::tcGET:
        client->sendDataToTcpClient();
        break;

    //
    case ATcp::tcPOST:
         client->storeInputData();
        break;

    //
    case ATcp::tcPOSTGET:
        client->storeInputData();
        client->sendDataToTcpClient();
        break;

    default:
        return;
        break;
    }
}



//------------------------------------------------------------------------------
// Обработка отключения клиентов
//------------------------------------------------------------------------------
void TcpServer::clientDisconnected_()
{
    QTcpSocket* sock = (QTcpSocket*) sender();

    AbstractClientDelegate* client = newClients_.value(sock, Q_NULLPTR);

    if (!client)
        return;

    newClients_.remove(sock);
    authorizedClients_.remove(client->getName());

    emit logPrint(ATcp::sc_OK_CLIENT_DISCONNECTED,
                  client->getName() % ":" % QString::number(client->getId()));

    delete client;
}




//------------------------------------------------------------------------------
// Обработка ошибок соединений
//------------------------------------------------------------------------------
void TcpServer::onAcceptError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);

    emit logPrint(ATcp::sc_ER_SERVER_INTERNAL_ERROR, errorString());
}
