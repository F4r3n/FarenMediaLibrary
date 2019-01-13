#include "Core/GameObject.h"
#include "Core/SceneManager.h"
#include "Core/Scene.h"
#include <nlohmann/json.hpp>

#include "Components/component.h"
#include "Components/CTransform.h"
#include "Components/CMaterial.h"
#include "Components/CMesh.h"
#include "Components/CCamera.h"
using namespace fm;
GameObject* GameObject::create()
{
    if(_entity != nullptr) return this;
    _entity = EntityManager::get().createEntity();
    std::shared_ptr<Scene> s = SceneManager::get().getCurrentScene();
    s->AddGameObject(this);
    return this;
}

GameObject* GameObject::create(std::shared_ptr<Scene> s)
{
    if(_entity != nullptr) return this;
    _entity = EntityManager::get().createEntity();
    s->AddGameObject(this);
    return this;
}

GameObject* GameObject::create(fm::Scene* s)
{
	if (_entity != nullptr) return this;
	_entity = EntityManager::get().createEntity();
	s->AddGameObject(this);
	return this;
}

GameObject::GameObject()
{
    name = "GameObject";
}

void GameObject::Serialize(json &outResult)
{
    std::vector<BaseComponent*> compos = getAllComponents();

	json compo;
    for(auto c : compos)
    {
        json j;
        if(c->Serialize(j))
        {
			compo[std::to_string(c->GetType())] = j;
        }
    }
	outResult["enabled"] = _entity->active;
	outResult["components"] = compo;
}


bool GameObject::Read(const json &inJson)
{
	_entity->active = inJson["enabled"];
	json compo = inJson["components"];
    for (nlohmann::json::const_iterator it = compo.cbegin(); it != compo.cend(); ++it)
    {
        switch(std::stoi(it.key()))
        {
            case fmc::ComponentType::kTransform:
                {
                fmc::CTransform * t = add<fmc::CTransform>();
                t->Read(it.value());
                }
            break;
            case fmc::ComponentType::kMaterial:
                {
                fmc::CMaterial * t = add<fmc::CMaterial>();
                t->Read(it.value());
                }
            break;
            case fmc::ComponentType::KMesh:
                {
                fmc::CMesh * t = add<fmc::CMesh>();
                t->Read(it.value());
                }
            break;
            case fmc::ComponentType::kCamera:
                {
                fmc::CCamera * t = add<fmc::CCamera>();
                t->Read(it.value());
                t->Init();
                }
            break;
        }

    }
    return true;
}

void GameObject::SetStatus(bool inStatus)
{
	_oldStatus = _entity->active;
	_entity->active = inStatus;
}

void GameObject::ResetStatus()
{
	_entity->active = _oldStatus;
}


GameObject::~GameObject() {}
