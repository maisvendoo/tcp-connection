// 29 11 2017

#include "tcp-client.h"

#include    <QTimer>
#include    <QTcpSocket>

#include "tcp-structs.h"


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TcpClient::TcpClient()
    : lastAuthResponse_(ATcp::ar_NO_RESONSE)
{

}



//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TcpClient::~TcpClient()
{

}



//------------------------------------------------------------------------------
//  Инициализация клиента
//------------------------------------------------------------------------------
void TcpClient::init(tcp_config_t tcp_config)
{
    // Сохраняем данные конфигурации
    this->tcp_config = tcp_config;

    // Создаем клиентский сокет
    socket = new QTcpSocket();
    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_4_0);

    // Создаём и настраиваем таймер-коннектор
    timerConnector_ = new QTimer(this);
    timerConnector_->setInterval(tcp_config.reconnect_interval);
    connect(timerConnector_, SIGNAL(timeout()),
            this, SLOT(onTimerConnector()));

    // Связываем сигналы и слоты

    // обработка соединения
    connect(socket, SIGNAL(connected()), this, SLOT(onConnect()));

    // обработка дисконнекта
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));

    // прием данных
    connect(socket, SIGNAL(readyRead()), this, SLOT(receive()));
}



//------------------------------------------------------------------------------
//  Проверка соединения
//------------------------------------------------------------------------------
bool TcpClient::isConnected() const
{
    return socket->state() == QTcpSocket::ConnectedState;
}



//------------------------------------------------------------------------------
//  Запуск клиента
//------------------------------------------------------------------------------
void TcpClient::start()
{
    timerConnector_->start();
}



//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
const tcp_config_t TcpClient::getConfig() const
{
    return tcp_config;
}



//------------------------------------------------------------------------------
//  (слот) Cоединение с сервером
//------------------------------------------------------------------------------
void TcpClient::connectToServer()
{
    // Сбрасываем сокет
    socket->abort();
    tcp_state.recv_count = tcp_state.send_count = 0;

    // Выполняем соединение
    socket->connectToHost(tcp_config.host_addr,
                          tcp_config.port,
                          QIODevice::ReadWrite);
}



//------------------------------------------------------------------------------
//  (слот) Передача данных серверу
//------------------------------------------------------------------------------
void TcpClient::sendToServer(QByteArray send_data)
{
    socket->write(send_data);
    socket->flush();

    tcp_state.send_count++;
}



//------------------------------------------------------------------------------
//  (слот) Прием данных от сервера
//------------------------------------------------------------------------------
void TcpClient::connectArrayPtr(QByteArray *&arrPtr)
{
    arrPtr = &incomingData_;
}



//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TcpClient::receive()
{
    // Читаем принятые данные
    incomingData_ = socket->readAll();

    // Наращиваем счетчик принятых пакетов
    tcp_state.recv_count++;

    // Генерация сигнала об успешной авторизации
    if (tcp_state.recv_count == 1)
    {
        ATcp::AuthResponse resp = ATcp::ar_NO_RESONSE;
        memcpy(&resp, incomingData_.data(), sizeof(ATcp::AuthResponse));

        lastAuthResponse_ = resp;
        switch (resp)
        {
        case ATcp::ar_AUTHORIZED:
            emit authorized();
            emit logPrint(ATcp::cc_OK_AUTHOROZED, tcp_config.name);
            break;

        case ATcp::ar_NAME_DUPLICATE:
            handleAuthError_(ATcp::cc_ER_NAME_DUPLICATE);
            break;

        case ATcp::ar_UNKNOWN_NAME:
            handleAuthError_(ATcp::cc_ER_UNKNOWN_NAME);
            break;

        default:
            lastAuthResponse_ = ATcp::ar_NO_RESONSE;
            handleAuthError_(ATcp::cc_UNKNOWN_CODE);
            break;
        }

        return;
    }

    emit dataReceived(incomingData_);
}



//------------------------------------------------------------------------------
//  (слот) Обработка факта соединения с сервером
//------------------------------------------------------------------------------
void TcpClient::onConnect()
{
    timerConnector_->stop();
    tcp_state.recv_count = tcp_state.send_count = 0;
    logPrint(ATcp::cc_OK_CONNECTED,
             QString::number(socket->socketDescriptor()));

    if (socket->isOpen())
    {      
        tcp_cmd_t cmd;
        cmd.info.command = ATcp::tcAUTHORIZATION;
        cmd.setData(tcp_config.name);

        sendToServer(cmd.toByteArray());
    }

    emit connectedToServer();
}



//------------------------------------------------------------------------------
//  (слот) Обработка факта разрыва соединения с сервером
//------------------------------------------------------------------------------
void TcpClient::onDisconnect()
{    
    emit disconnectedFromServer();

    socket->abort();

    // Сбрасываем счетчик
    tcp_state.recv_count = tcp_state.send_count = 0;

    // Очищаем массив данных
    incomingData_ = QByteArray(0, Qt::Uninitialized);

    //
    switch (lastAuthResponse_)
    {
    //
    case ATcp::ar_NO_RESONSE:
    case ATcp::ar_AUTHORIZED:
        timerConnector_->start();
        break;

    //
    default:
        break;
    }
}




void TcpClient::handleAuthError_(ATcp::ClientCodes _cl)
{
    emit authorizationDenied(lastAuthResponse_);
    emit logPrint(_cl, tcp_config.name);
    socket->disconnectFromHost();
}



//------------------------------------------------------------------------------
//  (слот) Отдать принятые данные другому классу
//------------------------------------------------------------------------------
void TcpClient::onTimerConnector()
{
    if (!isConnected())
    {
        this->connectToServer();
    }
}
