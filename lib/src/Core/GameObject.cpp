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
#include "Components/CPointLight.h"

using namespace fm;


GameObject::GameObject()
{
	_id = EntityManager::get().createEntity().id();
}

Entity GameObject::_GetEntity() const
{
	return EntityManager::get().GetEntity(_id);
}


void GameObject::Serialize(nlohmann::json &outResult) const
{
    std::vector<BaseComponent*>&& compos = getAllComponents();

	nlohmann::json compo;
    for(auto &&c : compos)
    {
		
		nlohmann::json j;
		bool ok = true;
		switch (c->GetType())
		{
		case fmc::ComponentType::kTransform:
			dynamic_cast<fmc::FMComponent<fmc::CTransform>*>(c)->Serialize(j);
			break;
		case fmc::ComponentType::kMaterial:
			dynamic_cast<fmc::FMComponent<fmc::CMaterial>*>(c)->Serialize(j);
				break;
		case fmc::ComponentType::KMesh:
			dynamic_cast<fmc::FMComponent<fmc::CMesh>*>(c)->Serialize(j);
				break;
		case fmc::ComponentType::kBody:
			dynamic_cast<fmc::FMComponent<fmc::CBody>*>(c)->Serialize(j);
				break;
		case fmc::ComponentType::kCollider:
			dynamic_cast<fmc::FMComponent<fmc::CCollider>*>(c)->Serialize(j);
				break;
		case fmc::ComponentType::kCamera:
			dynamic_cast<fmc::FMComponent<fmc::CCamera>*>(c)->Serialize(j);
				break;
		case fmc::ComponentType::kScriptManager:
			dynamic_cast<fmc::FMComponent<fmc::CScriptManager>*>(c)->Serialize(j);
				break;
		case fmc::ComponentType::kIdentity:
			dynamic_cast<fmc::FMComponent<fmc::CIdentity>*>(c)->Serialize(j);
				break;
		case fmc::ComponentType::kText:
			dynamic_cast<fmc::FMComponent<fmc::CText>*>(c)->Serialize(j);
				break;
		case fmc::ComponentType::kPointLight:
			dynamic_cast<fmc::FMComponent<fmc::CPointLight>*>(c)->Serialize(j);
				break;
		default:
			ok = false;
			assert(false);
		}

		if (ok)
		{
			compo[std::to_string(c->GetType())] = j;
		}
        
    }
	outResult["enabled"] = _active;
	outResult["components"] = compo;
	outResult["order"] = _order;
}


bool GameObject::Read(const nlohmann::json &inJson)
{
	_active = inJson["enabled"];
	const nlohmann::json compo = inJson["components"];
	try
	{
		_order = inJson.at("order");
	}
	catch (std::exception&)
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
			case fmc::ComponentType::kPointLight:
				add<fmc::CPointLight>()->Read(it.value());
				break;
			default:
				assert(false);
        }

    }
    return true;
}

std::vector<BaseComponent*> GameObject::getAllComponents() const 
{
	return _GetEntity().getAllComponents();
}

fmc::CIdentity* GameObject::_GetIdentity() const
{
	auto entity = _GetEntity();
	if (!entity.has<fmc::CIdentity>())
	{
		entity.addComponent<fmc::CIdentity>();
	}

	return entity.get<fmc::CIdentity>();
}


void GameObject::activate(bool value)
{
	_active = value;
	_GetIdentity()->SetActive(_active);
}

bool GameObject::IsActive() const
{
	return _active;
}

void GameObject::SetStatus(bool inStatus)
{
	_oldStatus = _active;
	activate(inStatus);
}

void GameObject::ResetStatus()
{
	activate(_oldStatus);
}

void GameObject::SetName(const std::string &inName)
{
	if (!_GetEntity().has<fmc::CIdentity>())
	{
		_GetEntity().addComponent<fmc::CIdentity>();
	}
	_GetEntity().get<fmc::CIdentity>()->SetNameEntity(inName);

}

const std::string& GameObject::GetName() const
{
	if (!_GetEntity().has<fmc::CIdentity>())
	{
		_GetEntity().addComponent<fmc::CIdentity>();
	}

	return _GetEntity().get<fmc::CIdentity>()->GetNameEntity();
}


GameObject::~GameObject() {}
