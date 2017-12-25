// 30/11/2017

#include "client-delegates.h"

#include <QTcpSocket>

#include "abstract-data-engine.h"
#include "tcp-structs.h" // delete ???


AbstractClientDelegate::AbstractClientDelegate(QObject *parent)
    : QObject(parent)
    , name_("")
    , localId_(0)
    , socket_(Q_NULLPTR)
    , engine_(Q_NULLPTR)
{

}




AbstractClientDelegate::~AbstractClientDelegate()
{
    delete engine_;
}




QString AbstractClientDelegate::getName() const
{
    return name_;
}




void AbstractClientDelegate::setName(QString name)
{
    name_ = std::move(name);
}




void AbstractClientDelegate::setSocket(QTcpSocket *sock)
{
    socket_ = sock;
    localId_ = sock->socketDescriptor();
}




qintptr AbstractClientDelegate::getId() const
{
    return localId_;
}




void AbstractClientDelegate::setDataEngine(AbstractDataEngine *engine)
{
    engine_ = engine;
}




QByteArray AbstractClientDelegate::getInputBuffer() const
{
    return engine_->getInputBuffer();
}





DummyDelegate::DummyDelegate(QObject* parent)
    : AbstractClientDelegate(parent)
{
    name_ = "dummy";
    engine_ = new NullDataEngine();
}

DummyDelegate::~DummyDelegate()
{
    // Ничего не делать
}

void DummyDelegate::setName(QString name)
{
    Q_UNUSED(name);
    // Ничего не делать
}

void DummyDelegate::setSocket(QTcpSocket *sock)
{
    Q_UNUSED(sock);
    // Ничего не делать
}

void DummyDelegate::storeInputData()
{
    // Ничего не делать
}

void DummyDelegate::setOutputBuffer(QByteArray buf)
{
    Q_UNUSED(buf)
    // Ничего не делать
}

void DummyDelegate::sendAuthorizationResponse(ATcp::AuthResponse resp)
{
    Q_UNUSED(resp);
    // Ничего не делать
}

void DummyDelegate::sendDataToTcpClient()
{
    // Ничего не делать
}





ClientDelegate::ClientDelegate(QObject* parent)
    : AbstractClientDelegate(parent)
{

}




ClientDelegate::~ClientDelegate()
{

}




void ClientDelegate::storeInputData()
{
    if (!socket_->isOpen())
        return;

    engine_->setInputData(socket_->readAll());
    emit dataReceived(engine_->getInputBuffer());
}




void ClientDelegate::setOutputBuffer(QByteArray buf)
{
    engine_->setOutputBuffer(std::move(buf));
}




void ClientDelegate::sendAuthorizationResponse(ATcp::AuthResponse resp)
{
    switch (resp)
    {

    case ATcp::ar_AUTHORIZED:
    case ATcp::ar_NAME_DUPLICATE:
    case ATcp::ar_UNKNOWN_NAME:
    {
        QByteArray arr(reinterpret_cast<const char*>(&resp),
                       sizeof(ATcp::AuthResponse));
        socket_->write(arr);
        socket_->flush();
        break;
    }

    default:
        break;
    }
}




void ClientDelegate::sendDataToTcpClient()
{
    if (socket_->isOpen())
    {
        socket_->write(engine_->getPreparedData());
        socket_->flush();
    }
}
