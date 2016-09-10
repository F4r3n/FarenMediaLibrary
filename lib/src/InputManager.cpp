
#include "InputManager.h"
#include <stdarg.h>
using namespace fm;

std::map<int, bool> InputManager::keys;
std::map<int, bool> InputManager::keysReleased;
InputManager InputManager::_instance;

float InputManager::posX = 0;
float InputManager::posY = 0;

InputManager::InputManager(Window& window)
{
    init(window);
}

void InputManager::init(Window& window)
{
    glfwSetKeyCallback(window.window, InputManager::key_callback);
    glfwSetCursorPosCallback(window.window, InputManager::cursor_position_callback);
    this->window = window.window;
}

void InputManager::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    posX = xpos;
    posY = ypos;
}

void InputManager::init(GLFWwindow* window)
{
    glfwSetKeyCallback(window, InputManager::key_callback);
    glfwSetCursorPosCallback(window, InputManager::cursor_position_callback);
    this->window = window;
}

InputManager::InputManager()
{
}

void InputManager::getMousePosition(double& posX, double& posY)
{
    glfwGetCursorPos(window, &posX, &posY);
}

float InputManager::getMousePositionX()
{
    return posX;
}

float InputManager::getMousePositionY()
{
    return posY;
}

int InputManager::worldKeyboard(int key)
{
    if(!typeKeyboard)
        return key;
    if(key == FM_KEY_A)
        return FM_KEY_Q;
    if(key == FM_KEY_Z)
        return FM_KEY_W;
    if(key == FM_KEY_W)
        return FM_KEY_Z;
    if(key == FM_KEY_Q)
        return FM_KEY_A;
    return key;
}

InputManager& InputManager::getInstance()
{
    return _instance;
}

bool InputManager::multipleKeysPressed(int number, ...)
{
    va_list ap;
    int j;
    bool val = true;
    va_start(ap, number); // Requires the last fixed parameter (to get the address)
    for(j = 0; j < number; j++) {
        int v = va_arg(ap, int);
        val &= glfwGetKey(window, v);
    }
    va_end(ap);
    return val;
}
void InputManager::getMousePosition(Vector2<double>& pos)
{
    glfwGetCursorPos(window, &pos.x, &pos.y);
}

bool InputManager::keyIsPressed(int key)
{
    return glfwGetKey(window, worldKeyboard(key));
}

bool InputManager::keyIsReleased(int key)
{
    bool v = keysReleased[key];
    keysReleased[key] = false;
    return v;
}

int InputManager::getMouseButton(int id)
{
    return glfwGetMouseButton(window, id);
}

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(keys[key] && action == GLFW_RELEASE) {
        keysReleased[key] = true;
    }
    keys[key] = (action == GLFW_PRESS) ? true : false;
}

void InputManager::pollEvents(Window& window)
{
    window.events();
}

InputManager::~InputManager()
{
}
