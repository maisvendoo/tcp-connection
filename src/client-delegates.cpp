// 30/11/2017

#include "client-delegates.h"

#include <QTcpSocket>

#include "abstract-data-engine.h"
#include "tcp-structs.h"


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

    return 0;
}




bool AbstractClientDelegate::checkSocket(QTcpSocket *sock) const
{

    return false;
}




void AbstractClientDelegate::setDataEngine(AbstractDataEngine *engine)
{

}




void AbstractClientDelegate::setInputBuffer(QByteArray buf)
{

}




QByteArray AbstractClientDelegate::getInputBuffer() const
{
    return QByteArray();
}





DummyClientDelegate::DummyClientDelegate()
{
    name_ = "dummy";
    engine_ = new NullDataEngine();
}

DummyClientDelegate::~DummyClientDelegate()
{
    // Ничего не делать
}

void DummyClientDelegate::setName(QString name)
{
    Q_UNUSED(name);
    // Ничего не делать
}

void DummyClientDelegate::setSocket(QTcpSocket *sock)
{
    Q_UNUSED(sock);
    // Ничего не делать
}

void DummyClientDelegate::storeInputData()
{
    // Ничего не делать
}

void DummyClientDelegate::setOutputBuffer(QByteArray buf)
{
    Q_UNUSED(buf)
    // Ничего не делать
}

void DummyClientDelegate::sendAuthorized()
{
    // Ничего не делать
}

void DummyClientDelegate::sendDataToTcpClient()
{
    // Ничего не делать
}