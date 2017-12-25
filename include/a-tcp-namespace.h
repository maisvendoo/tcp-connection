// 30/11/2017
#ifndef ATCPNAMESPACE_H
#define ATCPNAMESPACE_H

#include <QObject>

constexpr ptrdiff_t WORD_LEN = 4;
const char AUTH_WORD[WORD_LEN]{'a', 'u', 't', 'h'};
const char DENY_WORD[WORD_LEN]{'d', 'e', 'n', 'y'};


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
     * \brief Перечислитель типов сообщений лога Tcp-сервера
     */
    enum ServerCodes
    {
        sc_UNKNOWN_CODE,             ///< Неизвестная ситуация
        // OK сообщения
        sc_OK_SERVER_STARTED,        ///< Сервер запущен
        sc_OK_NEW_CLIENT_CONNECTED,  ///< Подключен новый клиент
        sc_OK_CLIENT_AUTHORIZED,     ///< Клиент авторизован
        sc_OK_CLIENT_DISCONNECTED,   ///< Клиент отключился
        // INFO сообщения
        sc_IN_AUTHORIZATION_REQUEST, ///< Клиент запрашивает авторизацию
        // ERROR сообщения
        sc_ER_SERVER_NOT_STARTED,    ///< Сервер не запущен
        sc_ER_CLIENT_NAME_DUPLICATE, ///< Дублирование имён клиентов
        sc_ER_CLIENT_UNKNOWN_NAME,   ///< Неизвестное имя клиента
        sc_ER_SERVER_INTERNAL_ERROR  ///< Внутренняя ошибка QTcpServer
    };
    Q_ENUM(ServerCodes)


    /*!
     * \enum ClientLogs
     * \brief Перечислитель типов сообщений лога Tcp-клиента
     */
    enum ClientCodes
    {
        cc_UNKNOWN_CODE,        ///< Неизвестная ситуация
        //
        cc_IN_AUTHORIZATION,
        // OK сообщения
        cc_OK_CONNECTED,
        cc_OK_AUTHOROZED,       ///< Клиент авторизован
        // ERROR сообщения
        cc_ER_NAME_DUPLICATE,   ///< Дублирование имён
        cc_ER_UNKNOWN_NAME      ///< Неизвестное имя клиента
    };
    Q_ENUM(ClientCodes)


    /*!
     * \enum AuthResponse
     * \brief Перечислитель кодов авторизации
     */
    enum AuthResponse
    {
        ar_NO_RESONSE = 0x0,     ///< Нулевое значение
        ar_AUTHORIZED = 0x1,     ///< Авторизован
        ar_NAME_DUPLICATE = 0x2, ///< Отказ авторизации, дублирование имён
        ar_UNKNOWN_NAME = 0x3    ///< Отказ авторизации, неизвестное имя
    };
    Q_ENUM(AuthResponse)

};

#endif // ATCPNAMESPACE_H
