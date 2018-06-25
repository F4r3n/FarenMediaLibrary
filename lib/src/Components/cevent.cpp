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

void CEvent::AddEvent(fm::BaseEvent *event)
{
     std::unordered_map<size_t, std::queue<fm::BaseEvent*>>::iterator it = _events.find(event->GetType());
    if(it != _events.end())
    {
        it->second.push(event);
    }else
    {
        size_t id = event->GetType();
        std::queue<fm::BaseEvent*> q;
        q.push(event);
        _events[id] = q;
    }
}


bool CEvent::Serialize(nlohmann::json &ioJson) const
{
    nlohmann::json j;
   //for(auto &s : _materials)
   //{
   //    nlohmann::json m = s;
   //   j.push_back(m);
   //}
   //ioJson["Materials"] = j;
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


