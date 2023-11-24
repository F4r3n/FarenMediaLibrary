#pragma once
#include "Script/Script.h"
#include <entt/entt.hpp>
class Entity;

namespace fm
{
class CppScript : public fm::Script
{
    public:
        CppScript(const std::string &nameClass);
        CppScript();
        ~CppScript();
        virtual bool init() override;
        virtual void start() override;
        virtual void update(float dt) override;
		virtual void Stop(const entt::handle& e) override;
        fm::Script::SCRIPT_TYPE GetType() const override
        {
            return fm::Script::SCRIPT_TYPE::CPP;
        }
        Behaviour* GetBehaviour() const;
		virtual bool Serialize(nlohmann::json &ioJson) const override;
		virtual bool Read(const nlohmann::json &inJSON) override;
    private:
        entt::handle currentEntity;
        Behaviour* behaviour;
};
}



