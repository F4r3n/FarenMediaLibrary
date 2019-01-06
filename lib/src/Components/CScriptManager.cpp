#include "Components/CScriptManager.h"
#include "Script/Script.h"
#include <EntityManager.h>
#include "Core/FilePath.h"
#include "Script/LuaScript.h"
using namespace fmc;

CScriptManager::CScriptManager() {
    _name = "ScriptManager";
}

CScriptManager::~CScriptManager() {
}

void CScriptManager::init( Entity* e) {
    for(auto &s : scripts) {
        s->init(e);
    }
}

void CScriptManager::Destroy()
{
    EntityManager::get().removeComponent<CScriptManager>(BaseComponent::_IDEntity);
}

void CScriptManager::RemoveScript(const std::string &name)
{
    size_t index = 0;
    size_t indexFound = 0;
    bool found = false;
    for(auto &s : scripts)
    {
        if(s->GetScriptName().compare(name) == 0)
        {
            indexFound = index;
            found = true;
            break;
        }
        index++;
    }
    if(found)
        scripts.erase(scripts.begin() + indexFound);
}



void CScriptManager::update(Entity *e, float dt)
{
    for(auto &s : scripts)
    {
        if(!s->isInit)
        {
            s->init(e);
        }

        if(!s->hasStarted)
        {
            s->start();
            s->hasStarted = true;
        }
        s->update(dt);
    }
}



void CScriptManager::addScriptLua(const fm::FilePath &inPath)
{
	Entity* e = EntityManager::get().getEntity(BaseComponent::_IDEntity);
	scripts.emplace_back(new fm::LuaScript(inPath, e));
}

void CScriptManager::ReloadScript(const std::string &inName)
{
	for (auto &s : scripts)
	{
		if (s->GetScriptName() == inName)
		{
			s->Reload();
		}
	}
}

