// 29/11/201
#ifndef ABSTRACT_ENGINE_DEFINER_H
#define ABSTRACT_ENGINE_DEFINER_H

#include <qcompilerdetection.h>

class AbstractDataEngine;

class AbstractEngineDefiner
{
public:
    ///
    AbstractEngineDefiner();
    ///
    virtual ~AbstractEngineDefiner();

    ///
    virtual AbstractDataEngine* getDataEngine(QString name) = 0;
};


class NullDataEngineDefiner Q_DECL_FINAL : public AbstractEngineDefiner
{
public:
    ///
    NullDataEngineDefiner();


protected:
    ///
    AbstractDataEngine* getDataEngine(quint64 id) Q_DECL_OVERRIDE;
};

#endif // ABSTRACT_ENGINE_DEFINER_H
