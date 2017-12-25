//------------------------------------------------------------------------------
//
//      Универсальный TCP-клиент для обмена данными приложений Qt
//      (c) РГУПС, ВЖД 31/03/2017
//      Разработал: Притыкин Д.Е.
//
//------------------------------------------------------------------------------
/*!
 * \file
 * \brief Универсальный TCP-клиент для обмена данными приложений Qt
 * \copyright РГУПС, ВЖД
 * \author Притыкин Д.Е.
 * \date 31/03/2017
 */

#include    "tcp-client.h"
#include    "tcp-server-structs.h"

#include    <QTimer>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TcpClient::TcpClient()
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
    timerConnector_->setInterval(tcp_config.ms_timeout);
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
const tcp_config_t TcpClient::getConfig()
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
void TcpClient::sendToServer(QByteArray &send_data)
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
        if (strcmp(incomingData_.data(), "authorized") == 0)
        {
            emit authorized();
        }
    }
}

//------------------------------------------------------------------------------
//  (слот) Обработка факта соединения с сервером
//------------------------------------------------------------------------------
void TcpClient::onConnect()
{
    timerConnector_->stop();
    tcp_state.recv_count = tcp_state.send_count = 0;

    if (socket->isOpen())
    {      
        client_cmd_t cmd;
        cmd.id = AUTHORIZATION;
        cmd.data_size = tcp_config.name.toUtf8().size() + 1;
        memcpy(cmd.data, tcp_config.name.toStdString().c_str(), cmd.data_size);

        QByteArray toSend = QByteArray::fromRawData((const char*) &cmd,
                                                    sizeof(client_cmd_t));

        sendToServer(toSend);
    }

    emit connectedToServer();
}

//------------------------------------------------------------------------------
//  (слот) Обработка факта разрыва соединения с сервером
//------------------------------------------------------------------------------
void TcpClient::onDisconnect()
{    
    emit disconnectedFromServer();

    // Сбрасываем счетчик
    tcp_state.recv_count = tcp_state.send_count = 0;

    // Очищаем массив данных
    incomingData_ = QByteArray(0, Qt::Uninitialized);

    timerConnector_->start();
}

//------------------------------------------------------------------------------
//  (слот) Послать состояние клиента другому классу
//------------------------------------------------------------------------------
//void TcpClient::sendTcpState(tcp_state_t *tcp_state)
//{
//    // Передаем наружу состояние клиента
//    *tcp_state = this->tcp_state;
//}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//void TcpClient::sendData(QByteArray *recv_data)
//{
//    // Передаем наружу принятые данные
//    *recv_data = this->incomingData_;
//}


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
