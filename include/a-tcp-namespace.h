// 30/11/2017
#ifndef ATCPNAMESPACE_H
#define ATCPNAMESPACE_H

#include <QObject>

class ATcp
{
    Q_GADGET

public:

    /*!
     * \enum TcpCommand enum
     * \brief Перечислитель команд клиента серверу
     */
    enum TcpCommand
    {
        tcZERO = 0x00,          ///< Ничего не делать
        tcAUTHORIZATION = 0x01, ///< Запрос авторизации
        tcGET = 0x02,       ///< Отправить данные без сохранения буфера запроса
        tcPOST = 0x03,      ///< Сохранить буфер запроса без отправки данных
        tcPOSTGET = 0x04    ///< Сохранить буфер запроса и отправить данные
    };
    Q_ENUM(TcpCommand)


    /*!
     * \enum ServerLogs enum
     * \brief Перечислитель типов сообщений лога сервера
     */
    enum ServerLogs
    {
        // OK сообщения
        OK_SERVER_STARTED,          ///< Сервер запущен
        OK_NEW_CLIENT_CONNECTED,    ///< Подключен новый клиент
        OK_CLIENT_AUTHORIZED,       ///< Клиент авторизован
        // INFO сообщения
        IN_AUTHORIZATION_REQUEST,   ///< Клиент запрашивает авторизацию
        // ERROR сообщения
        ER_SERVER_NOT_STARTED,      ///< Сервер не запущен
        ER_CLIENT_NAME_DUPLICATE,   ///< Дублирование имён клиентов
        ER_SERVER_INTERNAL_ERROR    ///< Внутренняя ошибка QTcpServer
    };
    Q_ENUM(ServerLogs)
};

#endif // ATCPNAMESPACE_H
