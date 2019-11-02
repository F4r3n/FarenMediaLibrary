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

void CEvent::AddEvent(fm::BaseEvent *inEvent)
{
	auto it = _events.find(inEvent->GetType());
	if (it != _events.end())
	{
		it->second.push(std::move(inEvent));
	}
	else
	{
		size_t id = inEvent->GetType();
		std::queue<fm::BaseEvent*> q;
		q.push(std::move(inEvent));
		_events[id] = q;
	}
}


bool CEvent::Serialize(nlohmann::json &ioJson) const
{
    return false;
}

bool CEvent::Read(const nlohmann::json &inJSON)
{
    return false;
}


const std::string& CEvent::GetName() const
{
    return kName;
}


