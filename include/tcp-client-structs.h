// 29 11 2017
#ifndef     TCP_CLIENT_STRUCTS_H
#define     TCP_CLIENT_STRUCTS_H

#include    <QtGlobal>

/*!
 *  \struct tcp_state_t
 *  \brief Состояние клиента
 */
//------------------------------------------------------------------------------
//      Состояние клиента
//------------------------------------------------------------------------------
struct tcp_state_t
{
    /// Счетчик переданных пакетов данных
    uint64_t    send_count;
    /// Счетчик принятых пакетов данных
    uint64_t    recv_count;

    tcp_state_t()
    {
        send_count = recv_count = 0;
    }
};


/*!
 *  \struct tcp_config_t
 *  \brief Конфигурация клиента
 */
//------------------------------------------------------------------------------
//      Конфигурация клиента
//------------------------------------------------------------------------------
struct tcp_config_t
{
    /// Имя клиента (сигнатура для идентификации на сервере)
    QString name;
    /// Адрес сервера
    QString host_addr;
    /// Порт, на котором слушает сревер
    quint16 port;
    /// Таймаут между попытками соедиения
    int     reconnect_interval;

    tcp_config_t()
    {
        name = "";
        host_addr = "127.0.0.1";
        port = 1992;
        reconnect_interval = 1000;
    }
};

#endif // TCP_CLIENT_STRUCTS_H
