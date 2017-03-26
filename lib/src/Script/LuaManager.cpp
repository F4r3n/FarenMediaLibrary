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
#include "Time.h"
#include "Physic/Collider.h"
#include "EventComponents.h"
Entity* createEntity() {
    return EntityManager::get().createEntity();
}

LuaManager LuaManager::instance;
using namespace fmc;
using namespace fm;

LuaManager::LuaManager() {}

LuaManager::~LuaManager() {}

void LuaManager::registerComponents() {
    registerComponent<math::Vector2f>("Vector2f",sol::constructors<sol::types<float, float>>(),
 "x", &math::Vector2f::x, "y", &math::Vector2f::y);
    registerComponent<math::Vector2d>("Vector2d", "x", &math::Vector2d::x, "y", &math::Vector2d::y);
    registerComponent<Vertex>("Vertex", "position", &Vertex::position, "uv", &Vertex::uv);

    registerComponent<Color>("Color", "r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a);
    registerComponent<CTransform>("CTransform",
                                  "position",
                                  &CTransform::position,
                                  "scale",
                                  &CTransform::scale,
                                  "rotation",
                                  &CTransform::rotation,
                                  "layer",
                                  &CTransform::layer);
    registerComponent<CMaterial>("CMaterial", "color", &CMaterial::color);
    registerComponent<CSource>("CSource", "play", &CSource::play, "status", &CSource::getStatus);
    registerComponent<CMesh>("CMesh",
    "setShape", &CMesh::setShape,
    "create", &CMesh::create,
    "clean", &CMesh::clean,
    "addVertex", sol::overload(&CMesh::addVertexPositionUV,&CMesh::addVertex, &CMesh::addVertexPositionUVVectors),
    "removeVertex", &CMesh::removeVertex,
    "removeVertices", &CMesh::removeVertices,
    "addIndex", &CMesh::addIndex,
    "removeIndice", &CMesh::removeIndice,
    "removeIndices", &CMesh::removeIndices,
    "update", &CMesh::update
    );
    registerComponent<Body2D>("Body2D", 
    "applyForceCenter", &Body2D::applyForceCenter2, 
    "friction", &Body2D::friction);

    registerComponent<Entity>("Entity",
                              "ID",
                              sol::readonly(&Entity::ID),
                              "getTransform",
                              &Entity::get<CTransform>,
                              "getMaterial",
                              &Entity::get<CMaterial>,
                              "getSource",
                              &Entity::get<CSource>,
                              "getBody",
                              &Entity::get<Body2D>,
                              
                              "addTransform",
                              &Entity::add<CTransform>,
                              "addMesh",
                              &Entity::add<CMesh>,
                              "addMaterial",
                              &Entity::add<CMaterial>,
                                "addBody",
                              &Entity::add<Body2D>,
                              
                              "removeTransform",
                              &Entity::remove<CTransform>,
                              "removeMesh",
                              &Entity::remove<CMesh>,
                              "removeMaterial",
                              &Entity::remove<CMaterial>
                              );
    registerComponent<Collision>("Collision", "ID", sol::readonly(&Collision::id));
    registerComponent<ColliderInfo>("ColliderInfo", "ID", sol::readonly(&ColliderInfo::idOther));

    registerComponent<Input>("Input",
                             "getMouseButton",
                             &Input::getMouseButton,
                             "keyIsPressed",
                             &Input::keyIsPressed,
                             "getMousePosition",
                             &Input::getMousePositionVector,
                             "getMouseButton",
                             &Input::getMouseButton);
       lua.new_usertype<Time>("Time",
                "dt", sol::var(std::ref(Time::dt)),
                "timeStamp", sol::var(std::ref(Time::timeStamp))
        );


    lua.set_function("createEntity", &createEntity);
}