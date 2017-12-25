//-----------------------------------------------------------------------------
//
//      Базовый класс подготовки данных для отправки клиентам
//      (c) РГУПС, ВЖД 13/07/2017
//      Разработал: Ковшиков С. В.
//
//-----------------------------------------------------------------------------
/*!
 * \file
 * \brief Базовый класс подготовки данных для отправки клиентам
 * \copyright РГУПС, ВЖД
 * \author Ковшиков С. В.
 * \date 13/07/2017
 */


#ifndef ABSTRACTDATAPREPAREENGINE_H
#define ABSTRACTDATAPREPAREENGINE_H

#include <QObject>
#include <QByteArray>

#include "tcp-server-structs.h"

/*!
 * \class AbstractDataPrepareEngine
 * \brief Реализация класса подготовки данных для отправки клиентам
 */
class AbstractDataPrepareEngine : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Конструкор
     * \param trainPtr - указатель на модель поезда
     */
    AbstractDataPrepareEngine();

    /// Вернуть подготовленные данные
    virtual QByteArray getPreparedData() = 0;

    /// Установить необходимые данные, принятые от клиента
    virtual void setDataFromClient(client_cmd_t dataFromClient);


signals:

    ///
    void requestEs2gDataFromServer(char* data);

};

#endif // ABSTRACTDATAPREPAREENGINE_H
