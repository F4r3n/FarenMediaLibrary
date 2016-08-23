#include "GameObject.h"

using namespace fm;
GameObject::GameObject() {

}

GameObject::~GameObject() {

}

bool GameObject::attachScript(const std::string &script){
	lua.script_file(script);
}