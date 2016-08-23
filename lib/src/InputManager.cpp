
#include "InputManager.h"
#include <stdarg.h>
using namespace fm;

std::map<int, bool> InputManager::keys;
std::map<int, bool> InputManager::keysReleased;
InputManager::InputManager(Window &window)
{
	
	glfwSetKeyCallback(window.window, InputManager::key_callback);
	this->window = window.window;
}

bool InputManager::multipleKeysPressed(int number,...) {
	va_list ap;
	int j;
	bool val = true;
	va_start(ap, number); //Requires the last fixed parameter (to get the address)
	for (j = 0; j < number; j++) {
		int v = va_arg(ap, int);
		val &= glfwGetKey(window, v);
	}
	va_end(ap);
	return val;
}
void InputManager::getMousePosition(Vector2<double> &pos) {
	glfwGetCursorPos(window, &pos.x, &pos.y);
}

bool InputManager::keyIsPressed(int key) {
	return glfwGetKey(window, key);
}

bool InputManager::keyIsReleased(int key) {
	bool v = keysReleased[key];
	keysReleased[key] = false;
	return v;
}

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (keys[key] && action == GLFW_RELEASE) {
		keysReleased[key] = true;
	}
	keys[key] = (action == GLFW_PRESS) ? true: false;
	
}

void InputManager::pollEvents(Window &window) {
	window.events();
}

InputManager::~InputManager()
{
}
