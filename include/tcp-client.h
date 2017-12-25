// 29 11 2017

#ifndef		TCPCLIENT_H
#define 	TCPCLIENT_H

#include	<QtGlobal>
#include    <QObject>
#include    <QDataStream>

#include "tcp-client-structs.h"
#include "a-tcp-namespace.h"

class QTimer;
class QTcpSocket;

struct tcp_cmd_t;

#if defined(TCPCONNECTION_LIB)
# define TCPCLIENT_EXPORT Q_DECL_EXPORT
#else
# define TCPCLIENT_EXPORT Q_DECL_IMPORT
#endif


/*!
 *  \class TcpClient
 *  \brief Класс, обслуживающий клиентское TCP/IP-соединение
 */
//-----------------------------------------------------------------------------
//	Класс, обслуживающий клиентское TCP/IP-соединение
//-----------------------------------------------------------------------------
class TCPCLIENT_EXPORT TcpClient Q_DECL_FINAL : public QObject
{
    Q_OBJECT

public:
    /// Конструктор и деструктор
    explicit TcpClient();
    ~TcpClient();

    /// Инициализация клиента
    void init(tcp_config_t tcp_config);

    /// Проверка соединения
    bool isConnected() const;

    /// Запуск клиента
    void start();

    ///
    const tcp_config_t getConfig() const;

    ///
    void sendToServer(ATcp::TcpCommand comm, QByteArray data);

    /// Передача данных серверу
    void sendToServer(tcp_cmd_t &cmd);


signals:
    ///
    void connectedToServer();

    ///
    void disconnectedFromServer();

    ///
    void authorized();

    ///
    void authorizationDenied(ATcp::AuthResponse errId);

    ///
    void dataReceived(QByteArray inData);

    ///
    void logPrint(ATcp::ClientCodes logId, QString msg = "");


public slots:
    /// Cоединение с сервером
    void connectToServer();

    /// Прием данных от сервера
    void receive();

    /// Обработка факта соединения с сервером
    void slotConnect();

    /// Обработка факта разрыва соединения с сервером
    void slotDisconnect();


protected:
    /// Клиентский сокет
    QTcpSocket      *socket;

    /// Входящий поток данных
    QDataStream     in;

    /// Текущее состояние клиента
    tcp_state_t     tcp_state;

    /// Конфигурация клиента
    tcp_config_t    tcp_config;

    /// Данные, принятые от сервера
    QByteArray      incomingData_;

    ///
    ATcp::AuthResponse lastAuthResponse_;


private:
    ///
    QTimer* timerConnector_;

    ///
    void handleAuthError_(ATcp::ClientCodes _cl);


private slots:
    ///
    void sendToServer_(QByteArray send_data);

    ///
    void onTimerConnector();

};

#endif
