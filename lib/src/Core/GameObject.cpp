#include "Core/GameObject.h"
#include "Core/SceneManager.h"
#include "Core/Scene.h"
#include <nlohmann/json.hpp>

#include "Components/component.h"
#include "Components/CTransform.h"
#include "Components/CMaterial.h"
#include "Components/CMesh.h"
#include "Components/CCamera.h"
#include "Components/CBody.h"
#include "Components/CCollider.h"
#include "Components/CScriptManager.h"
#include "Components/CIdentity.h"
#include "Components/CText.h"

using namespace fm;


GameObject::GameObject()
{
	_entity = EntityManager::get().createEntity();

}

void GameObject::Serialize(nlohmann::json &outResult) const
{
    std::vector<BaseComponent*>&& compos = getAllComponents();

	nlohmann::json compo;
    for(auto &&c : compos)
    {
		nlohmann::json j;
        if(c->Serialize(j))
        {
			compo[std::to_string(c->GetType())] = j;
        }
    }
	outResult["enabled"] = _entity->active;
	outResult["components"] = compo;
	outResult["order"] = _order;
}


bool GameObject::Read(const nlohmann::json &inJson)
{
	_entity->active = inJson["enabled"];
	const nlohmann::json compo = inJson["components"];
	try
	{
		_order = inJson.at("order");
	}
	catch (std::exception& e)
	{

	}
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
			case fmc::ComponentType::kBody:
				add<fmc::CBody>()->Read(it.value());
			break;
			case fmc::ComponentType::kCollider:
				add<fmc::CCollider>()->Read(it.value());
			break;
            case fmc::ComponentType::kCamera:
                add<fmc::CCamera>()->Read(it.value());
            break;
			case fmc::ComponentType::kScriptManager:
				add<fmc::CScriptManager>()->Read(it.value());
			break;
			case fmc::ComponentType::kIdentity:
				add<fmc::CIdentity>()->Read(it.value());
			break;
			case fmc::ComponentType::kText:
				add<fmc::CText>()->Read(it.value());
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

void GameObject::SetName(const std::string &inName)
{
	if (!_entity->has<fmc::CIdentity>())
	{
		_entity->addComponent<fmc::CIdentity>();
	}
	_entity->get<fmc::CIdentity>()->SetNameEntity(inName);

}

const std::string& GameObject::GetName() const
{
	if (!_entity->has<fmc::CIdentity>())
	{
		_entity->addComponent<fmc::CIdentity>();
	}

	return _entity->get<fmc::CIdentity>()->GetNameEntity();
}


GameObject::~GameObject() {}
