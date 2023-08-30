#include "Core/Scene.h"
#include "Core/GameObject.h"
#include <nlohmann/json.hpp>
#include "Components/CTransform.h"
#include "Components/CIdentity.h"
#include <fstream>
using namespace fm;

Scene::Scene(const fm::FilePath &inPath) : fm::Observable("Scene"), _path(inPath), _name(inPath.GetName(true))
{
}
Scene::Scene(const std::string& inName) : fm::Observable("Scene"), _name(inName)
{

}

Scene::~Scene()
{
}

void Scene::Load()
{
	std::ifstream i(_path.GetPath());
	nlohmann::json json;
	i >> json;

	Read(json);
	_isDirty = false;
}

void Scene::Save()
{
	nlohmann::json json;
	Serialize(json);

	std::ofstream o(_path.GetPath());
	o << json << std::endl;

	_isDirty = false;
}

void Scene::Destroy()
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
	Entity::Id id = o->getID();
	_gos[o->getID()] = move(o);
	_isDirty = true;
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
		go.second->SetStatus(inStatus);
	}
}

void Scene::ResetStatusGo()
{
	for (auto& go : _gos)
	{
		go.second->ResetStatus();
	}
}

std::shared_ptr<fm::GameObject> Scene::GetGameObjectByID(Entity::Id inID)
{
	auto it = _gos.find(inID);
	if (it != _gos.end())
	{
		return it->second;
	}
	return nullptr;
}


void Scene::DeleteGameObjectByID(Entity::Id inID)
{
	auto it = _gos.find(inID);
	if (it != _gos.end())
	{
		it->second->destroy();
		_gos.erase(inID);
	}
	_isDirty = true;

	NotifyAll(fm::EventObserver((size_t)Event::DELETE_GAMEOBJECT,inID));
}

