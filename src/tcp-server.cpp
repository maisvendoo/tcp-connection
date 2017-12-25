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




void TcpServer::start(quint16 port)
{
    if (!isListening())
    {
        if (listen(QHostAddress::Any, port))
        {
            emit logPrint(ATcp::OK_SERVER_STARTED, QString::number(port));
        }
        else
        {
            emit logPrint(ATcp::ER_SERVER_NOT_STARTED, QString::number(port));
        }
    }
}




void TcpServer::setEngineDefiner(AbstractEngineDefiner *definer)
{
    if (engineDefiner_)
        delete engineDefiner_;

    engineDefiner_ = std::move(definer);
}




AbstractClientDelegate *TcpServer::getClient(QString clientName)
{
    return authorizedClients_.value(clientName, dummyClient_);
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

    emit logPrint(ATcp::ER_SERVER_INTERNAL_ERROR, errorString());
}




void TcpServer::receive_()
{
    //
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());

    if (!tcp_cmd_t::validInfoSize(sock))
        return;

    ATcp::TcpCommand cmd = tcp_cmd_t::extractCommand(sock);

    size_t bufSize = tcp_cmd_t::extractBufferSize(sock);

    if(!tcp_cmd_t::validBufferSize(bufSize, sock))
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
        authorizeClient_(client, sock->readAll());
        break;

    case ATcp::tcGET:
        client->sendDataToTcpClient();
        break;

    case ATcp::tcPOST:
         client->storeInputData();
        break;

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
