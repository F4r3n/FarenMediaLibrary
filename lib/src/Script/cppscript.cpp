#include "Script/cppscript.hpp"
#include <ECS.h>
#include <Entity.h>
#include "Script/cppmanager.hpp"
#include "Core/Debug.h"
#include <Components/CTransform.h>
using namespace fm;

CppScript::CppScript(const std::string &nameClass)
{
    _scriptName = nameClass;
}

CppScript::CppScript()
{

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
    _isInit = true;
    if(behaviour == nullptr)
        fm::Debug::get().LogError("Unable to create the  class");
    return true;
}

Behaviour* CppScript::GetBehaviour() const
{
    return behaviour;
}

bool CppScript::Serialize(nlohmann::json &ioJson) const
{
	//Set values from inspector
	return false;
}

bool CppScript::Read(const nlohmann::json &inJSON)
{
	return false;
}

void CppScript::start()
{
    if(behaviour)
    {
		//fmc::CTransform *t = behaviour->entity->get<fmc::CTransform>();
        behaviour->Start();
        _hasStarted = true;
    }
}

void CppScript::update(float dt)
{
    if(behaviour)
    {
		
        behaviour->Update();
    }
}

void CppScript::Stop(Entity* e)
{

}

