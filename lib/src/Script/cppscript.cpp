#include "Script/cppscript.hpp"
#include <ECS.h>
#include <Entity.h>
#include "Script/cppmanager.hpp"
#include "Core/Debug.h"
#include <Components/CTransform.h>
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
    isInit = true;
    if(behaviour == nullptr)
        fm::Debug::get().LogError("Unable to create the  class");
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
		fmc::CTransform *t = behaviour->entity->get<fmc::CTransform>();
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
