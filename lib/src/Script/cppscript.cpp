#include "Script/cppscript.hpp"
#include <ECS.h>
#include <Entity.h>
#include "Script/cppmanager.hpp"
#include "Core/Debug.h"
using namespace fm;

CppScript::CppScript(const std::string &nameClass)
{
    this->_scriptName = nameClass;
}

CppScript::~CppScript()
{
   delete behaviour;
}

bool CppScript::init(Entity* e)
{

    currentEntity = e;
    behaviour = CPPManager::get().InstantiateClass(_scriptName);
    behaviour->entity = currentEntity;
    std::cout << "INIT " << behaviour << std::endl;
    isInit = true;
    if(behaviour == nullptr)
        fm::Debug::log("error");
    return true;
}

Behaviour* CppScript::GetBehaviour() const
{
    return behaviour;
}


void CppScript::start()
{
    if(behaviour)
    {
        behaviour->Start();
        hasStarted = true;
    }
}

void CppScript::update()
{
    if(behaviour)
    {
        behaviour->Update();
    }
}
