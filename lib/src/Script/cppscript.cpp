#include "Script/cppscript.hpp"
#include <ECS.h>
#include <Entity.h>
#include "Script/cppmanager.hpp"
using namespace fm;

CppScript::CppScript(const std::__cxx11::string &nameClass)
{
    this->nameClass = nameClass;
}

CppScript::~CppScript()
{

}

bool CppScript::init(Entity* e)
{
    currentEntity = e;
    behaviour = CPPManager::get().InstantiateClass(nameClass);
    return true;
}

void CppScript::start()
{
    behaviour->Start();
}

void CppScript::update()
{
    if(behaviour)
    {
        behaviour->Update();
    }
}
