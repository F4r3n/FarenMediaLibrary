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
	Clear();
}

void CEvent::Destroy()
{
    EntityManager::get().removeComponent<CEvent>(BaseComponent::_IDEntity);
}

void CEvent::AddEvent(fm::BaseEvent *inEvent)
{
	auto &it = _events[inEvent->GetType()];
	it.emplace_back(std::move(inEvent));
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

void CEvent::Clear()
{
	for (auto && events : _events)
	{
		for (auto &&e : events)
		{
			delete e;
		}
		events.clear();
	}
}


