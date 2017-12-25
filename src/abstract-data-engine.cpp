//-----------------------------------------------------------------------------
//
//      Базовый класс подготовки данных для отправки клиентам
//      (c) РГУПС, ВЖД 29/11/201
//      Разработал: Ковшиков С. В.
//
//-----------------------------------------------------------------------------
/*!
 * \file
 * \brief Базовый класс подготовки данных для отправки клиентам
 * \copyright РГУПС, ВЖД
 * \author Ковшиков С. В.
 * \date 29/11/201
 */


#include "abstract-data-engine.h"



//-----------------------------------------------------------------------------
// КОНСТРУКТОР
//-----------------------------------------------------------------------------
AbstractDataEngine::AbstractDataEngine()
    : QObject(Q_NULLPTR)
{

}




AbstractDataEngine::~AbstractDataEngine()
{

}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AbstractDataEngine::setInputData(QByteArray inData)
{
//    if (inData.size() == inputBuffer_.size())
//    {
//        memcpy(inputBuffer_.data(), inData.data(), inputBuffer_.size());
//    }
    inputBuffer_ = std::move(inData);
}




void AbstractDataEngine::setOutputBuffer(QByteArray outData)
{
//    if (outData.size() == outputBuffer_.size())
//    {
//        memcpy(outputBuffer_.data(), outData.data(), outputBuffer_.size());
//    }
    outputBuffer_ = std::move(outData);
}




QByteArray AbstractDataEngine::getInputBuffer() const
{
    return inputBuffer_;
}




QByteArray AbstractDataEngine::getOutputData() const
{
    return outputBuffer_;
}



//-----------------------------------------------------------------------------
//
//      Класс отсутствия подготовки данных
//      (c) РГУПС, ВЖД 29/11/2017
//      Разработал: Ковшиков С. В.
//
//-----------------------------------------------------------------------------
/*!
 * \file
 * \brief Класс отсутствия подготовки данных
 * \copyright РГУПС, ВЖД
 * \author Ковшиков С. В.
 * \date 29/11/201
 */


//-----------------------------------------------------------------------------
// КОНСТРУКТОР
//-----------------------------------------------------------------------------
NullDataEngine::NullDataEngine()
    : AbstractDataEngine()
{

}



//-----------------------------------------------------------------------------
// Вернуть пустой массив данных
//-----------------------------------------------------------------------------
QByteArray NullDataEngine::getPreparedData()
{
    return QByteArray();
}
