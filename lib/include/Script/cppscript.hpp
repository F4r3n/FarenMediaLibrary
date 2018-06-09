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
        bool init(Entity* e) override;
        void start() override;
        void update() override;
        fm::Script::SCRIPT_TYPE GetType() const override
        {
            return fm::Script::SCRIPT_TYPE::CPP;
        }
        Behaviour* GetBehaviour() const;

    private:
        Entity* currentEntity;
        Behaviour* behaviour;
};
}

#endif // CPPSCRIPT_HPP

