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
        virtual size_t GetType() const =0;

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

        void AddEvent(fm::BaseEvent *event);
    private:
        std::unordered_map<size_t, std::queue<fm::BaseEvent*>> _events;
};


}
#endif // CEVENT_HPP

