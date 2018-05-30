#ifndef CPPSCRIPT_HPP
#define CPPSCRIPT_HPP
#include "Script/Script.h"

class Entity;

class CppScript : public fm::Script
{
    public:
        CppScript(const std::string &nameClass);
        CppScript();
        ~CppScript();
        bool init(Entity* e) override;
        void start() override;
        void update() override;
        virtual fm::Script::SCRIPT_TYPE GetType()
        {
            return fm::Script::SCRIPT_TYPE::CPP;
        }
    private:
        Entity* currentEntity;
        Behaviour* behaviour;
        std::string nameClass;
};

#endif // CPPSCRIPT_HPP

