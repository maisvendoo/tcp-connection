// 30/11/2017

#include "client-delegates.h"

#include <QTcpSocket>

#include "abstract-data-engine.h"
#include "tcp-structs.h" // delete ???


AbstractClientDelegate::AbstractClientDelegate()
    : name_("")
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
}




qintptr AbstractClientDelegate::getSocketDescriptor() const
{
    if (socket_)
        return socket_->socketDescriptor();

    return 0;
}




bool AbstractClientDelegate::checkSocket(QTcpSocket *sock) const
{
    if (socket_ && sock)
        return socket_ == sock;

    return false;
}




void AbstractClientDelegate::setDataEngine(AbstractDataEngine *engine)
{
    engine_ = engine;
}




void AbstractClientDelegate::setInputBuffer(QByteArray buf)
{
    engine_->setInputData(std::move(buf));
}




QByteArray AbstractClientDelegate::getInputBuffer() const
{
    return engine_->getInputBuffer();
}





DummyDelegate::DummyDelegate()
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

void DummyDelegate::sendAuthorized()
{
    // Ничего не делать
}

void DummyDelegate::sendDataToTcpClient()
{
    // Ничего не делать
}





ClientDelegate::ClientDelegate()
{

}




ClientDelegate::~ClientDelegate()
{

}




void ClientDelegate::storeInputData()
{
    if (socket_->state() != QTcpSocket::ListeningState)
        return;

    QByteArray buf = socket_->readAll();

    if (buf.size() != sizeof(tcp_cmd_t)) // FIXME delete - перенести в сервер!
        return;

    engine_->setInputData(socket_->readAll());
}




void ClientDelegate::setOutputBuffer(QByteArray buf)
{
    engine_->setOutputBuffer(std::move(buf));
}




void ClientDelegate::sendAuthorized()
{
    if (socket_->isOpen())
    {
        QByteArray arr("auth");
        socket_->write(arr);
        socket_->flush();
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
