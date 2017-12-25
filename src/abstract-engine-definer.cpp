#include "abstract-engine-definer.h"

#include "abstract-data-engine.h"
#include "null-data-engine.h"


AbstractEngineDefiner::AbstractEngineDefiner()
{

}




AbstractEngineDefiner::~AbstractEngineDefiner()
{

}




NullDataEngineDefiner::NullDataEngineDefiner()
{

}




AbstractDataEngine *NullDataEngineDefiner::getDataEngine(quint64 id)
{
    Q_UNUSED(clientName);
    return new NullDataEngine();
}
