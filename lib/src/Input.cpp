#include "Input.h"

using namespace fm;
bool Input::keyIsPressed(int key) {
	return InputManager::getInstance().keyIsPressed(key);
}

 bool Input::keyIsReleased(int key) {
	return InputManager::getInstance().keyIsReleased(key);
}
 void Input::getMousePosition(Vector2<double> &pos) 	{
 	InputManager::getInstance().getMousePosition(pos);
}