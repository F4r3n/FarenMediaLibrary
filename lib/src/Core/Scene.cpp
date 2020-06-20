#include "Core/Scene.h"
#include "Core/GameObject.h"
#include <nlohmann/json.hpp>
#include "Components/CTransform.h"
#include "Components/CIdentity.h"
using namespace fm;

Scene::Scene(const std::string &name) : fm::Observable("Scene")
{
    _name = name;
}


Scene::~Scene()
{
}

void Scene::destroy()
{
    for(auto &&e : _gos)
    {
        e.second->destroy();
    }
	_gos.clear();
}


std::shared_ptr<fm::GameObject> Scene::CreateGameObject(bool defaultValue)
{
	_UniqueIDScene++;

	std::shared_ptr<fm::GameObject> o = std::make_shared<fm::GameObject>();
	o->SetOrder(_UniqueIDScene);
	if (defaultValue)
	{
		o->addComponent<fmc::CTransform>(fm::math::Vector3f(0, 0, 0), fm::math::Vector3f(1, 1, 1), fm::math::vec3(0, 0, 0));
		o->addComponent<fmc::CIdentity>();
		o->get<fmc::CIdentity>()->SetNameEntity("GameObject");
	}
	ecs::id id = o->getID();
	_gos[o->getID()] = move(o);

	NotifyAll(fm::EventObserver((size_t)Event::CREATE_GAMEOBJECT, id));
	return GetGameObjectByID(id);
}

void Scene::Serialize(nlohmann::json &outJson)
{
    nlohmann::json entities;
    for(auto e : _gos)
    {
        nlohmann::json v;
        e.second->Serialize(v);
        entities.push_back(v);
    }
    outJson["entities"] = entities;
    outJson["id"] = 0;
}

bool Scene::Read(const nlohmann::json &inJson)
{
    nlohmann::json entities = inJson["entities"];

    for (nlohmann::json::const_iterator it = entities.cbegin(); it != entities.cend(); ++it)
    {
		std::shared_ptr<GameObject> go = CreateGameObject(false);
        go->Read(*it);
    }

    return true;
}

void Scene::SetStatusToGo(bool inStatus)
{
	for (auto& go : _gos)
	{
		go.second->SetStatus(false);
	}
}

void Scene::ResetStatusGo()
{
	for (auto& go : _gos)
	{
		go.second->ResetStatus();
	}
}

std::shared_ptr<fm::GameObject> Scene::GetGameObjectByID(ecs::id inID)
{
	auto it = _gos.find(inID);
	if (it != _gos.end())
	{
		return it->second;
	}
	return nullptr;
}


void Scene::DeleteGameObjectByID(ecs::id inID)
{
	auto it = _gos.find(inID);
	if (it != _gos.end())
	{
		it->second->destroy();
		_gos.erase(inID);
	}
	NotifyAll(fm::EventObserver((size_t)Event::DELETE_GAMEOBJECT,inID));
}

