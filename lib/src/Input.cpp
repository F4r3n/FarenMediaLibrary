#include "Input.h"

using namespace fm;
bool Input::keyIsPressed(int key) {
	//std::cout << "Called " << key << std::endl;
	//std::cout << InputManager::getInstance().keyIsPressed(key) << std::endl;
	return InputManager::getInstance().keyIsPressed(key);
}

 bool Input::keyIsReleased(int key) {
	return InputManager::getInstance().keyIsReleased(key);
}

int Input::getMouseButton(int id) {
	return InputManager::getInstance().getMouseButton(id);
}
 void Input::getMousePosition(Vector2<double> &pos) 	{
 	InputManager::getInstance().getMousePosition(pos);
}