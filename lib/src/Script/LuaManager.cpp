#include <sol2/sol.hpp>
#include "Script/LuaManager.h"
#include "Input/Input.h"
#include "Core/Math/Vector2.h"
#include "Components/CTransform.h"
#include "Core/Color.h"
#include "Components/CMaterial.h"
#include <chrono>
#include "EntityManager.h"
#include <cassert>
#include "Components/CMesh.h"
#include "Components/CSource.h"
#include "Components/CCollider.h"
#include "EventComponents.h"
#include "Components/CCamera.h"
#include "Core/Rect.h"
#include "Resource/ResourcesManager.h"
#include "Core/Debug.h"
#include "Components/CIdentity.h"
#include "Components/CBody.h"

GameObjectLua::GameObjectLua(Entity* inEntity)
{
	_entity = inEntity;
}

fmc::CTransform* GameObjectLua::GetTransform()
{
	return _entity->get<fmc::CTransform>();
}

const char* GameObjectLua::GetName()
{
	return _entity->get<fmc::CIdentity>()->GetNameEntity().c_str();
}

fmc::CBody* GameObjectLua::GetBody()
{
	return _entity->get<fmc::CBody>();
}



Entity* createEntity()
{
  return EntityManager::get().createEntity();
}

void Log(const std::string &inMessage)
{
	fm::Debug::get().LogError(inMessage);
}

LuaManager LuaManager::instance;
using namespace fmc;
using namespace fm;

LuaManager::LuaManager() 
{
	lua = new sol::state;
}

LuaManager::~LuaManager()
{
	delete lua;
}

void LuaManager::_OpenInternalLibs()
{
	FilePath p = fm::ResourcesManager::GetFilePathResource(fm::LOCATION::INTERNAL_LUA_LOCATION);
	Folder f(p);
	f.Iterate(true, [this](const fm::Folder* inFolder, const fm::File* inFile)
		{
			if (inFile != nullptr)
			{
				lua->script_file(inFile->GetPath().GetPath(), sol::load_mode::text);
			}
		}
	);
}

bool LuaManager::ReadFile(const std::string &inPath)
{
	bool result = false;
	try
	{
		result = lua->safe_script_file(inPath).valid();
	}
	catch (std::exception &)
	{
		throw;
	}
	return result;
}


void LuaManager::openLibraries()
{
	lua->open_libraries(sol::lib::base, sol::lib::package);
	_OpenInternalLibs();
	registerComponents();
}

void LuaManager::registerComponents() 
{
	lua->set_function("Log", &Log);
	lua->new_usertype<GameObjectLua>("GameObjectInternal",
		"GetTransform", &GameObjectLua::GetTransform,
		"GetBody3D", &GameObjectLua::GetBody,
		"GetName", &GameObjectLua::GetName
		);

	lua->new_usertype<math::Vector2f>("Vector2f",sol::constructors<sol::types<float, float>>(),
	 "x", &math::Vector2f::x, "y", &math::Vector2f::y);
	lua->new_usertype<math::vec3>("Vector3",sol::constructors<sol::types<float, float, float>>(),
	 "x", &math::vec3::x, "y", &math::vec3::y, "z", &math::vec3::z);
	lua->new_usertype<math::Vector2d>("Vector2d", "x", &math::Vector2d::x, "y", &math::Vector2d::y);
	lua->new_usertype<Rectf>("Rectf", "x", &Rectf::x, "y", &Rectf::y, "w", &Rectf::w, "h", &Rectf::h);
	
	lua->new_usertype<Bounds>("Bounds",
	  "center", sol::property(&Bounds::getCenter, &Bounds::setCenter),
	  "scale", sol::property(&Bounds::getScale, &Bounds::setScale),
	  "size", sol::property(&Bounds::getSize, &Bounds::setSize)
	  );
	lua->new_usertype<Color>("Color", "r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a);


	lua->new_usertype<CTransform>("CTransform",
		"position",
		&CTransform::position,
		"scale",
		&CTransform::scale,
		"rotation",
		&CTransform::GetRotation,
	  "layer",
	  &CTransform::layer);
	lua->new_usertype<CCamera>("CCamera",
	  "viewPort", &CCamera::GetViewport,
	  "isOrthographic", &CCamera::IsOrthographic
	  );
	lua->new_usertype<CSource>("CSource", "play", &CSource::play, "status", &CSource::getStatus);

	//lua->new_usertype<Body2D>("Body2D",
	//  "applyForceCenter", &Body2D::ApplyForceCenter2,
	//  "setFriction", &Body2D::SetFriction);

	lua->new_usertype<CBody>("CBody",
		"setLinearVelocity", &CBody::SetLinearVelocity,
		"getLinearVelocity", &CBody::GetLinearVelocity,
		"getGravity", &CBody::GetGravity,
		"setGravity", &CBody::SetGravity,
		"getMass", &CBody::GetMass,
		"setMass", &CBody::SetMass
		);

	lua->new_usertype<InputManager>("Input",
		"isKeyPressed", &InputManager::IsKeyPressed,
		"isMouseButtonPressed", &InputManager::IsMouseButtonPressed);

	(*lua)["Input"] = &InputManager::Get();

}
