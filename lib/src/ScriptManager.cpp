#include "ScriptManager.h"
#include "Input.h"
using namespace fm;
ScriptManager::ScriptManager() {
	lua.open_libraries();
}

int keyboard(int id) {
    return fm::InputManager::getInstance().keyIsPressed(id);
}

void ScriptManager::init() {
	lua.set_function("keyIsPressed", &Input::keyIsPressed);
    lua.set_function("getMousePositionX", &Input::getMousePositionX);
    lua.set_function("getMousePositionY", &Input::getMousePositionY);

    //lua.set_function("getMousePosition", &Input::getMousePosition);

	for(auto &s : scripts) {
		s.second->init(lua);
	}
}

void ScriptManager::update(float dt) {
for(auto &s : scripts) {
		s.second->update(lua, dt);
	}
}

std::shared_ptr<Script> ScriptManager::getScript(const std::string &name) {
	return scripts[name];
}

void ScriptManager::registerScript(const std::string &name) {
	scripts[name] = std::make_shared<Script>(name);
}


void ScriptManager::start() {
for(auto &s : scripts) {
		s.second->start(lua);
	}
}

ScriptManager::~ScriptManager() {

}