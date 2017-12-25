// 29/11/201
#ifndef ABSTRACT_ENGINE_DEFINER_H
#define ABSTRACT_ENGINE_DEFINER_H

#include <qcompilerdetection.h>

class AbstractDataEngine;
class AbstractClientDelegate;

#if defined(TCPCONNECTION_LIB)
    #define ENGINE_DEFINER_EX Q_DECL_EXPORT
#else
    #define ENGINE_DEFINER_EX Q_DECL_IMPORT
#endif


class ENGINE_DEFINER_EX AbstractEngineDefiner
{
public:
    ///
    AbstractEngineDefiner();
    ///
    virtual ~AbstractEngineDefiner();

    ///
    void setDataEngine(AbstractClientDelegate* client);


protected:
    ///
    virtual AbstractDataEngine* getDataEngine_(QString name) = 0;
};


class NullDataEngineDefiner Q_DECL_FINAL : public AbstractEngineDefiner
{
public:
    ///
    NullDataEngineDefiner();


protected:
    ///
    AbstractDataEngine* getDataEngine_(QString clientName) Q_DECL_OVERRIDE;
};

#endif // ABSTRACT_ENGINE_DEFINER_H
