#ifndef CEVENT_HPP
#define CEVENT_HPP
#include "component.h"
#include <array>
#include <unordered_map>
#include <queue>

namespace fm
{
	class BaseEvent
	{
	    public:
	        BaseEvent();
	        virtual ~BaseEvent();
	        virtual size_t GetType() const = 0;
	};

	enum EventKind
	{
		COLLISION,
		LAST
	};
}

namespace fmc
{
	typedef std::vector<fm::BaseEvent*> EventVector;
	typedef std::array<EventVector, fm::EventKind::LAST> Events;

class CEvent
{
    public:
        CEvent();
        ~CEvent();
        bool Serialize(nlohmann::json &ioJson) const;
        bool Read(const nlohmann::json &inJSON);
        const std::string& GetName() const;
		uint16_t GetType() const {return kEvent;}

		void AddEvent(fm::BaseEvent *inEvent);
		Events GetEvents() const { return _events; }
		void Clear();

    private:
		Events _events;
private:
	std::string _name;
};


}
#endif // CEVENT_HPP

