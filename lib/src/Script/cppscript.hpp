#ifndef CPPSCRIPT_HPP
#define CPPSCRIPT_HPP
#include "Script/Script.h"

class Entity;

namespace fm
{
class CppScript : public fm::Script
{
    public:
        CppScript(const std::string &nameClass);
        CppScript();
        ~CppScript();
        virtual bool init(Entity* e) override;
        virtual void start() override;
        virtual void update(float dt) override;
		virtual void Stop(Entity* e) override;
        fm::Script::SCRIPT_TYPE GetType() const override
        {
            return fm::Script::SCRIPT_TYPE::CPP;
        }
        Behaviour* GetBehaviour() const;
		virtual bool Serialize(nlohmann::json &ioJson) const override;
		virtual bool Read(const nlohmann::json &inJSON) override;
    private:
        Entity* currentEntity;
        Behaviour* behaviour;
};
}

#endif // CPPSCRIPT_HPP

