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

class CEvent : public FMComponent<CEvent>
{
    public:
        CEvent();
        ~CEvent();
        bool Serialize(nlohmann::json &ioJson) const override;
        bool Read(const nlohmann::json &inJSON) override;
        const std::string& GetName() const override;
		uint16_t GetType() const override {return kEvent;}
        void Destroy() override;

		void AddEvent(fm::BaseEvent *inEvent);
		Events GetEvents() const { return _events; }
		void Clear();

    private:
		Events _events;
};


}
#endif // CEVENT_HPP

