#include "Components/cevent.hpp"
#include <EntityManager.h>
#include <nlohmann/json.hpp>
using namespace fmc;
static const std::string kName = "Event";


namespace fm
{

BaseEvent::BaseEvent() {}
BaseEvent::~BaseEvent() {}
}

CEvent::CEvent()
{
}


CEvent::~CEvent()
{
}

void CEvent::Destroy()
{
    EntityManager::get().removeComponent<CEvent>(BaseComponent::_IDEntity);
}




bool CEvent::Serialize(nlohmann::json &ioJson) const
{
    nlohmann::json j;

    return true;
}

bool CEvent::Read(const nlohmann::json &inJSON)
{

    return true;
}


const std::string& CEvent::GetName() const
{
    return kName;
}


