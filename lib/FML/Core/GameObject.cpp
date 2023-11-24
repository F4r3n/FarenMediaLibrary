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


GameObject::GameObject(entt::handle inHandle)
{
	_entity = inHandle;
	++_ID;
}

void GameObject::Serialize(nlohmann::json &outResult) const
{	
	nlohmann::json compo;
    for(size_t i = 0; i < fmc::ComponentType::kEND; ++i)
    {
		nlohmann::json j;
		bool ok = true;
		switch (i)
		{
		case fmc::ComponentType::kTransform:
			if(auto component = _entity.try_get<fmc::CTransform>(); component != nullptr)
				component->Serialize(j);
			break;
		case fmc::ComponentType::kMaterial:
			if (auto component = _entity.try_get<fmc::CMaterial>(); component != nullptr)
				component->Serialize(j);
				break;
		case fmc::ComponentType::KMesh:
			if (auto component = _entity.try_get<fmc::CMesh>(); component != nullptr)
				component->Serialize(j);
				break;
		case fmc::ComponentType::kBody:
			if (auto component = _entity.try_get<fmc::CBody>(); component != nullptr)
				component->Serialize(j);
				break;
		case fmc::ComponentType::kCollider:
			if (auto component = _entity.try_get<fmc::CCollider>(); component != nullptr)
				component->Serialize(j);
				break;
		case fmc::ComponentType::kCamera:
			if (auto component = _entity.try_get<fmc::CCamera>(); component != nullptr)
				component->Serialize(j);
				break;
		case fmc::ComponentType::kScriptManager:
			if (auto component = _entity.try_get<fmc::CScriptManager>(); component != nullptr)
				component->Serialize(j);
				break;
		case fmc::ComponentType::kIdentity:
			if (auto component = _entity.try_get<fmc::CIdentity>(); component != nullptr)
				component->Serialize(j);
				break;
		case fmc::ComponentType::kText:
			if (auto component = _entity.try_get<fmc::CText>(); component != nullptr)
				component->Serialize(j);
				break;
		case fmc::ComponentType::kPointLight:
			if (auto component = _entity.try_get<fmc::CPointLight>(); component != nullptr)
				component->Serialize(j);
				break;
		default:
			ok = false;
			assert(false);
		}

		if (ok)
		{
			compo[std::to_string(i)] = j;
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
				add<fmc::CTransform>().Read(it.value());
            break;
            case fmc::ComponentType::kMaterial:
				add<fmc::CMaterial>().Read(it.value());
            break;
            case fmc::ComponentType::KMesh:
				add<fmc::CMesh>().Read(it.value());
            break;
			case fmc::ComponentType::kBody:
				add<fmc::CBody>().Read(it.value());
			break;
			case fmc::ComponentType::kCollider:
				add<fmc::CCollider>().Read(it.value());
			break;
            case fmc::ComponentType::kCamera:
                add<fmc::CCamera>().Read(it.value());
            break;
			case fmc::ComponentType::kScriptManager:
				add<fmc::CScriptManager>().Read(it.value());
			break;
			case fmc::ComponentType::kIdentity:
				add<fmc::CIdentity>().Read(it.value());
			break;
			case fmc::ComponentType::kText:
				add<fmc::CText>().Read(it.value());
			break;
			case fmc::ComponentType::kPointLight:
				add<fmc::CPointLight>().Read(it.value());
				break;
			default:
				assert(false);
        }

    }
    return true;
}


fmc::CIdentity& GameObject::_GetIdentity() const
{
	if (!has<fmc::CIdentity>())
	{
		add<fmc::CIdentity>();
	}

	return get<fmc::CIdentity>();
}


void GameObject::activate(bool value)
{
	_active = value;
	_GetIdentity().SetActive(_active);
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
	if (!has<fmc::CIdentity>())
	{
		addComponent<fmc::CIdentity>();
	}
	get<fmc::CIdentity>().SetNameEntity(inName);

}

const std::string& GameObject::GetName() const
{
	if (!has<fmc::CIdentity>())
	{
		add<fmc::CIdentity>();
	}

	return get<fmc::CIdentity>().GetNameEntity();
}


GameObject::~GameObject() {}
