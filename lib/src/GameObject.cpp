#include "GameObject.h"

using namespace fm;
int idKey = 0;

GameObject::GameObject() {
lua.open_libraries( sol::lib::base, sol::lib::coroutine, sol::lib::string, sol::lib::io, sol::lib::package);
}

GameObject::~GameObject() {

}

void GameObject::start() {
    lua["start"]();
    transform.position.x = lua["transform"]["position"]["x"];
    transform.position.y = lua["transform"]["position"]["y"];
}

void GameObject::update(float dt) {
	lua["update"](dt);

	transform.position.x = lua["transform"]["position"]["x"];
	transform.position.y = lua["transform"]["position"]["y"];
}

int keyboard(int id) {
    return fm::InputManager::getInstance().keyIsPressed(id);
}


Transform GameObject::getTransform() const {
	return transform;
}


bool GameObject::attachScript(const std::string &script){
	lua.script_file(script);
   
	

	lua.set_function("keyIsPressed", keyboard);
    lua.create_named_table("transform",
        "position", lua.create_table_with(
                "x", 0,
                // can reference other existing stuff too
                "y", 0),
        "scale", lua.create_table_with(
                "x", 0,
                // can reference other existing stuff too
                "y", 0
        )
		);

      return true;
}