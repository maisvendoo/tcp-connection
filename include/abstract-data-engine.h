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


#ifndef ABSTRACT_DATA_ENGINE_H
#define ABSTRACT_DATA_ENGINE_H

#include <QObject>
#include <QByteArray>


#if defined(TCPCONNECTION_LIB)
    #define DATA_ENGINE_EX Q_DECL_EXPORT
#else
    #define DATA_ENGINE_EX Q_DECL_IMPORT
#endif


/*!
 * \class AbstractDataPrepareEngine
 * \brief Реализация класса подготовки данных для отправки клиентам
 */
class DATA_ENGINE_EX AbstractDataEngine : public QObject
{
    Q_OBJECT
public:
    /// Конструктор
    AbstractDataEngine();
    ///
    ~AbstractDataEngine();

    /// Вернуть подготовленные данные
    virtual QByteArray getPreparedData() = 0;

    /// Установить необходимые данные, принятые от клиента
    void setInputData(QByteArray inData);

    ///
    void setOutputBuffer(QByteArray outData);

    ///
    QByteArray getInputBuffer() const;

    ///
    QByteArray getOutputBuffer() const;


signals:
    ///
    void requestDataFromServer(char* data);

private:
    ///
    QByteArray inputBuffer_;
    ///
    QByteArray outputBuffer_;
};



/*!
 * \class NullDataPrepareEngine
 * \brief Реализация класса отсутствия подготовки данных
 */
class DATA_ENGINE_EX NullDataEngine Q_DECL_FINAL : public AbstractDataEngine
{
    Q_OBJECT
public:
    /*!
     * \brief Конструктор
     * \param trainPtr - указатель на модель поезда
     */
    NullDataEngine();

    /// Вернуть пустой массив данных
    QByteArray getPreparedData() Q_DECL_OVERRIDE;

};
#endif // ABSTRACT_DATA_ENGINE_H
