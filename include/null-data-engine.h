//-----------------------------------------------------------------------------
//
//      Класс отсутствия подготовки данных
//      (c) РГУПС, ВЖД 13/07/2017
//      Разработал: Ковшиков С. В.
//
//-----------------------------------------------------------------------------
/*!
 * \file
 * \brief Класс отсутствия подготовки данных
 * \copyright РГУПС, ВЖД
 * \author Ковшиков С. В.
 * \date 13/07/2017
 */

#ifndef NULLDATAPREPAREENGINE_H
#define NULLDATAPREPAREENGINE_H

#include "abstract-data-prepare-engine.h"


/*!
 * \class NullDataPrepareEngine
 * \brief Реализация класса отсутствия подготовки данных
 */
class NullDataPrepareEngine Q_DECL_FINAL : public AbstractDataPrepareEngine
{
    Q_OBJECT
public:
    /*!
     * \brief Конструктор
     * \param trainPtr - указатель на модель поезда
     */
    NullDataPrepareEngine();

    /// Вернуть пустой массив данных
    QByteArray getPreparedData() Q_DECL_OVERRIDE;

};

#endif // NULLDATAPREPAREENGINE_H
