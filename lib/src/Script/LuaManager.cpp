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
#include "Components/Body2D.h"
#include "Physic/Collider.h"
#include "EventComponents.h"
#include "Components/CCamera.h"
#include "Core/Rect.h"
#include "Resource/ResourcesManager.h"


GameObjectLua::GameObjectLua(Entity* inEntity)
{
	_entity = inEntity;
}

fmc::CTransform* GameObjectLua::GetTransform()
{
	return _entity->get<fmc::CTransform>();
}


Entity* createEntity()
{
  return EntityManager::get().createEntity();
}

LuaManager LuaManager::instance;
using namespace fmc;
using namespace fm;

LuaManager::LuaManager() 
{
	lua = new sol::state;
}

LuaManager::~LuaManager() {}

void LuaManager::_OpenInternalLibs()
{
	FilePath p = fm::ResourcesManager::GetFilePathResource(fm::ResourcesManager::INTERNAL_LUA_LOCATION);

	std::vector<FilePath> paths;
	FilePath::GetAllFiles(p, ".lua", true, paths);

	for (const FilePath& file : paths)
	{
		lua->script_file(file.GetPath(), sol::load_mode::text);
	}
}

bool LuaManager::ReadFile(const std::string &inPath)
{
	bool result = false;
	try
	{
		result = lua->safe_script_file(inPath).valid();
	}
	catch (std::exception &e)
	{
		throw e;
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
  
	lua->new_usertype<GameObjectLua>("GameObjectInternal",
		"GetTransform", &GameObjectLua::GetTransform
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
		&CTransform::rotation,
	  "layer",
	  &CTransform::layer);
	lua->new_usertype<CCamera>("CCamera",
	  "viewPort", &CCamera::viewPort,
	  "isOrthographic", &CCamera::IsOrthographic
	  );
	lua->new_usertype<CSource>("CSource", "play", &CSource::play, "status", &CSource::getStatus);
	  //registerComponent<CMesh>("CMesh",
	  //"setShape", &CMesh::setShape,
	  //"create", &CMesh::create,
	  //"clean", &CMesh::clean,
	  //"addVertex", sol::overload(&CMesh::addVertexPositionUV,&CMesh::addVertex, &CMesh::addVertexPositionUVVectors),
	  //"removeVertex", &CMesh::removeVertex,
	  //"removeVertices", &CMesh::removeVertices,
	  //"addIndex", &CMesh::addIndex,
	  //"removeIndice", &CMesh::removeIndice,
	  //"removeIndices", &CMesh::removeIndices,
	  //"update", &CMesh::update,
	  //"bounds", &CMesh::bounds
	  //);
	lua->new_usertype<Body2D>("Body2D",
	  "applyForceCenter", &Body2D::ApplyForceCenter2,
	  "setFriction", &Body2D::SetFriction);

}
