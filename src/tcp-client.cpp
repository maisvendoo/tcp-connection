//-----------------------------------------------------------------------------
//
//      Класс TCP клиента для общения с TCP сервером
//      (c) РГУПС, ВЖД 29/11/2017
//      Разработал: Ковшиков С. В., Притыкин Д. Е.
//
//-----------------------------------------------------------------------------
/*!
 *  \file
 *  \brief Класс TCP клиента для общения с TCP сервером
 *  \copyright РГУПС, ВЖД
 *  \author Ковшиков С. В., Притыкин Д. Е.
 *  \date 29/11/2017
 */

#include "tcp-client.h"

#include    <QTimer>
#include    <QTcpSocket>

#include "tcp-structs.h"


//------------------------------------------------------------------------------
// Конструктор
//------------------------------------------------------------------------------
TcpClient::TcpClient()
    : lastAuthResponse_(ATcp::ar_NO_RESONSE)
{

}



//------------------------------------------------------------------------------
// Деструктор
//------------------------------------------------------------------------------
TcpClient::~TcpClient()
{

}



//------------------------------------------------------------------------------
// Инициализация клиента
//------------------------------------------------------------------------------
void TcpClient::init(tcp_config_t tcp_config)
{
    // Сохраняем данные конфигурации
    this->tcp_config = tcp_config;

    // Создаем клиентский сокет
    socket = new QTcpSocket(this);
    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_4_0);

    // Создаём и настраиваем таймер-коннектор
    timerConnector_ = new QTimer(this);
    timerConnector_->setInterval(tcp_config.reconnect_interval);
    connect(timerConnector_, SIGNAL(timeout()),
            this, SLOT(onTimerConnector()));

    // Связываем сигналы и слоты

    // Обработка соединения
    connect(socket, SIGNAL(connected()), this, SLOT(slotConnect()));

    // Обработка дисконнекта
    connect(socket, SIGNAL(disconnected()), this, SLOT(slotDisconnect()));

    // Прием данных
    connect(socket, SIGNAL(readyRead()), this, SLOT(receive()));
}



//------------------------------------------------------------------------------
// Проверка соединения
//------------------------------------------------------------------------------
bool TcpClient::isConnected() const
{
    return socket->state() == QTcpSocket::ConnectedState;
}



//------------------------------------------------------------------------------
// Запуск клиента
//------------------------------------------------------------------------------
void TcpClient::start()
{
    timerConnector_->start();
}



//------------------------------------------------------------------------------
// Вернуть структуру состояния клиента
//------------------------------------------------------------------------------
const tcp_config_t TcpClient::getConfig() const
{
    return tcp_config;
}



//-----------------------------------------------------------------------------
// Отправить данные на сервер с заданной командой
//-----------------------------------------------------------------------------
void TcpClient::sendToServer(ATcp::TcpCommand comm, QByteArray data)
{
    // Формируем tcp_cmd_t структуру из данной команды и массива байт
    tcp_cmd_t cmd;
    cmd.info.command = comm;
    cmd.setData(data);
    sendToServer_(cmd.toByteArray());
}



//-----------------------------------------------------------------------------
// Передача данных серверу
//-----------------------------------------------------------------------------
void TcpClient::sendToServer(tcp_cmd_t &cmd)
{
    sendToServer_(cmd.toByteArray());
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
// Прием данных от сервера
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
        // Читаем код ответа из буффера входящих данных
        memcpy(&resp, incomingData_.data(), sizeof(ATcp::AuthResponse));

        // Запоминаем код ответа авторизации
        lastAuthResponse_ = resp;

        switch (resp)
        {
        // Если успешно авторизован
        case ATcp::ar_AUTHORIZED:
            emit authorized();
            emit logPrint(ATcp::cc_OK_AUTHOROZED, tcp_config.name);
            break;

        // Если дублирование имени
        case ATcp::ar_NAME_DUPLICATE:
            handleAuthError_(ATcp::cc_ER_NAME_DUPLICATE);
            break;

        // Если неизвестное имя клиента
        case ATcp::ar_UNKNOWN_NAME:
            handleAuthError_(ATcp::cc_ER_UNKNOWN_NAME);
            break;

        default:
            lastAuthResponse_ = ATcp::ar_NO_RESONSE;
            handleAuthError_(ATcp::cc_UNKNOWN_CODE);
            break;
        }

        // Выходим, чтобы не оповещать о текущем приёме данных!
        return;
    }

    // Оповещаем о приёме данных
    emit dataReceived(incomingData_);
}



//------------------------------------------------------------------------------
// (слот) Обработка факта соединения с сервером
//------------------------------------------------------------------------------
void TcpClient::slotConnect()
{
    // Прекращаем попытки подключения
    timerConnector_->stop();
    // Сбрасываем счётчики
    tcp_state.recv_count = tcp_state.send_count = 0;
    // Вызываем сигнал печати лога
    emit logPrint(ATcp::cc_OK_CONNECTED, tcp_config.name);

    if (socket->isOpen())
    {      
        // Формируем команду авторизации и отправляем
        tcp_cmd_t cmd;
        cmd.info.command = ATcp::tcAUTHORIZATION;
        cmd.setData(tcp_config.name);

        sendToServer_(cmd.toByteArray());
    }

    // Оповещаем о подключении к серверу
    emit connectedToServer();
}



//------------------------------------------------------------------------------
// (слот) Обработка факта разрыва соединения с сервером
//------------------------------------------------------------------------------
void TcpClient::slotDisconnect()
{    
    emit disconnectedFromServer();

    socket->abort();

    // Сбрасываем счетчик
    tcp_state.recv_count = tcp_state.send_count = 0;

    // Очищаем массив данных
    incomingData_ = QByteArray(0, Qt::Uninitialized);

    switch (lastAuthResponse_)
    {
    // Если неизвестная ошибка или был успешно авторизован
    case ATcp::ar_NO_RESONSE:
    case ATcp::ar_AUTHORIZED:
        // Снова пытаемся подключиться
        timerConnector_->start();
        break;

    default:
        break;
    }
}



//-----------------------------------------------------------------------------
// Обработать ошибку авторизации
//-----------------------------------------------------------------------------
void TcpClient::handleAuthError_(ATcp::ClientCodes _cl)
{
    emit authorizationDenied(lastAuthResponse_);
    emit logPrint(_cl, tcp_config.name);
    socket->disconnectFromHost();
}



//------------------------------------------------------------------------------
// (слот) Передача данных серверу
//------------------------------------------------------------------------------
void TcpClient::sendToServer_(QByteArray send_data)
{
    socket->write(send_data);
    socket->flush();

    tcp_state.send_count++;
}



//------------------------------------------------------------------------------
// (слот) Отдать принятые данные другому классу
//------------------------------------------------------------------------------
void TcpClient::onTimerConnector()
{
    if (!isConnected())
    {
        this->connectToServer();
    }
}
