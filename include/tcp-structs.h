//29 11 2017
#ifndef TCPSTRUCTS_H
#define TCPSTRUCTS_H

#include "a-tcp-namespace.h"

#include <stddef.h>
#include <stdint.h>
#include <QByteArray>


constexpr size_t TCP_BUFFER_SIZE = 1024;

#pragma pack(push, 1)
struct tcp_cmd_t
{
    ATcp::TcpCommand command;
    size_t           data_size;
    uint8_t          buffer[TCP_BUFFER_SIZE]; // попробовать перевести на вектор !!!

    tcp_cmd_t()
        : command(ATcp::tcZERO)
        , data_size(0)
    {

    }

    QByteArray toByteAray()
    {
        QByteArray buf(sizeof *this, Qt::Uninitialized);
        memcpy(buf.data(), this, sizeof *this);
        return buf;
    }
};
#pragma pack(pop)

#endif // TCPSTRUCTS_H
