#include "Core/GameObject.h"
#include "Core/SceneManager.h"
#include "Core/Scene.h"
#include <nlohmann/json.hpp>

#include "Components/component.h"
#include "Components/CTransform.h"
#include "Components/CMaterial.h"
#include "Components/CMesh.h"
#include "Components/CCamera.h"
#include "Components/CBody3D.h"
#include "Components/CCollider.h"
using namespace fm;
size_t GameObject::_counter = 0;
GameObject* GameObject::create()
{
    if(_entity != nullptr) 
		return this;

    _entity = EntityManager::get().createEntity();
    std::shared_ptr<Scene> s = SceneManager::get().getCurrentScene();
    s->AddGameObject(this);

    return this;
}

GameObject* GameObject::create(std::shared_ptr<Scene> s)
{
    if(_entity != nullptr) 
		return this;

    _entity = EntityManager::get().createEntity();
    s->AddGameObject(this);

    return this;
}

GameObject* GameObject::create(fm::Scene* s)
{
	if (_entity != nullptr) 
		return this;

	_entity = EntityManager::get().createEntity();
	s->AddGameObject(this);

	return this;
}

GameObject::GameObject()
{
    name = "GameObject " + _counter;
}

void GameObject::Serialize(json &outResult) const
{
    std::vector<BaseComponent*>&& compos = getAllComponents();

	json compo;
    for(auto &&c : compos)
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
	const json compo = inJson["components"];
    for (nlohmann::json::const_iterator it = compo.cbegin(); it != compo.cend(); ++it)
    {
        switch(std::stoi(it.key()))
        {
            case fmc::ComponentType::kTransform:
				add<fmc::CTransform>()->Read(it.value());
            break;
            case fmc::ComponentType::kMaterial:
				add<fmc::CMaterial>()->Read(it.value());
            break;
            case fmc::ComponentType::KMesh:
				add<fmc::CMesh>()->Read(it.value());
            break;
			case fmc::ComponentType::kBody3D:
				add<fmc::CBody3D>()->Read(it.value());
			break;
			case fmc::ComponentType::kCollider:
				add<fmc::CCollider>()->Read(it.value());
			break;
            case fmc::ComponentType::kCamera:
                add<fmc::CCamera>()->Read(it.value());
            break;
        }

    }
    return true;
}

std::vector<BaseComponent*> GameObject::getAllComponents() const 
{
	return _entity->getAllComponents();
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
