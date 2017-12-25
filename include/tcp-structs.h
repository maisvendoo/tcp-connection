//29 11 2017
#ifndef TCPSTRUCTS_H
#define TCPSTRUCTS_H

#include "a-tcp-namespace.h"

#include <type_traits>
#include <stddef.h>
#include <stdint.h>
#include <QByteArray>
#include <QTcpSocket>


struct tcp_cmd_t
{
#pragma pack(push, 1)
    struct info_t
    {
        ATcp::TcpCommand command;
        size_t bufferSize;

        info_t()
            : command(ATcp::tcZERO)
            , bufferSize(0u)
        {

        }
    };
#pragma pack(pop)

    info_t      info;
    QByteArray  buffer;

    static const size_t INFO_SIZE = sizeof(info_t);

    // Конструктор
    tcp_cmd_t()
    {

    }

    //
    void setData(const char* _null_term_str)
    {
        setData(std::move(QByteArray(_null_term_str)));
    }

    //
    void setData(const char* _dat, size_t _len)
    {
        setData(std::move(QByteArray(_dat, static_cast<int>(_len))));
    }

    //
    void setData(QString _dat)
    {
        setData(_dat.toLocal8Bit());
    }

    //
    void setData(QByteArray _dat)
    {
        buffer = _dat;
        info.bufferSize = buffer.size();
    }

    //
    template<typename T>
    bool setData(T _dat)
    {
        if (std::is_trivially_copyable<T>::value)
        {
            buffer.resize(sizeof(T));
            memcpy(buffer.data(), &_dat, sizeof(T));
            info.bufferSize = sizeof(T);
            return true;
        }
        return false;
    }

    //
    QByteArray toByteArray()
    {
        QByteArray buf(static_cast<int>(INFO_SIZE + info.bufferSize),
                       Qt::Uninitialized);

        memcpy(buf.data(), &info, sizeof(info_t));
        memcpy(buf.data() + INFO_SIZE, buffer.data(), info.bufferSize);

        return buf;
    }

    //
    static bool validInfoSize(QTcpSocket* _sock)
    {
        return static_cast<size_t>(_sock->size()) >= INFO_SIZE;
    }

    //
    static info_t extractInfo(QTcpSocket *_sock)
    {
        info_t tmp;
        _sock->read(reinterpret_cast<char*>(&tmp), sizeof(info_t));
        return tmp;
    }

    //
    static bool validBufferSize(size_t _bufSz, QTcpSocket* _sock)
    {
        return _bufSz == static_cast<size_t>(_sock->size());
    }
};

#endif // TCPSTRUCTS_H
