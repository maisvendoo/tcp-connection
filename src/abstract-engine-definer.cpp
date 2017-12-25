#include "abstract-engine-definer.h"

#include "client-delegates.h"
#include "abstract-data-engine.h"


AbstractEngineDefiner::AbstractEngineDefiner()
{

}




AbstractEngineDefiner::~AbstractEngineDefiner()
{

}

void AbstractEngineDefiner::setDataEngine(AbstractClientDelegate *client)
{
    client->setDataEngine(getDataEngine_(client->getName()));
}




NullDataEngineDefiner::NullDataEngineDefiner()
{

}




AbstractDataEngine *NullDataEngineDefiner::getDataEngine_(QString clientName)
{
    Q_UNUSED(clientName);
    return new NullDataEngine();
}
