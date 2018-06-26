#ifndef CEVENT_HPP
#define CEVENT_HPP
#include "component.h"
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
}

namespace fmc
{
class CEvent : public FMComponent<CEvent>
{
    public:
        CEvent();
        ~CEvent();
        bool Serialize(nlohmann::json &ioJson) const override;
        bool Read(const nlohmann::json &inJSON) override;
        const std::string& GetName() const override;
        size_t GetType() const override {return kEvent;}
        void Destroy() override;

        template <typename T, typename... Args>
        void AddEvent(Args&&...args)
        {
            std::unique_ptr<fm::BaseEvent> e = std::make_unique<T>(args...);
             std::unordered_map<size_t, std::queue<std::unique_ptr<fm::BaseEvent>>>::iterator it = _events.find(e->GetType());
            if(it != _events.end())
            {
                it->second.push(e);
            }else
            {
                size_t id = e->GetType();
                std::queue<std::unique_ptr<fm::BaseEvent>> q;
                q.push(e);
                _events[id] = q;
            }
        }
    private:
        std::unordered_map<size_t, std::queue<std::unique_ptr<fm::BaseEvent>>> _events;
};


}
#endif // CEVENT_HPP

