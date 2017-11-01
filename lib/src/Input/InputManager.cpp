
#include "Input/InputManager.h"
#include <stdarg.h>
#include <iostream>
using namespace fm;

std::map<int, bool> InputManager::_keys;
std::map<int, bool> InputManager::_keysReleased;
InputManager InputManager::_instance;

float InputManager::_posX = 0;
float InputManager::_posY = 0;

InputManager::InputManager(Window& window) {
    init(window);
}

void InputManager::init(Window& window) {
    // glfwSetKeyCallback(window.window, InputManager::key_callback);
    // glfwSetCursorPosCallback(window.window, InputManager::cursor_position_callback);
    // this->window = window.window;
}

// void InputManager::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
//{
//    posX = xpos;
//    posY = ypos;
//}

InputManager::InputManager() {
}
//TODO keyboard

void InputManager::getMousePosition(double& posX, double& posY) {
    int tempX, tempY;
    SDL_GetMouseState(&tempX, &tempY);
    posX = tempX;
    posY = tempY;
}

float InputManager::getMousePositionX() {
    return _posX;
}

float InputManager::getMousePositionY() {
    return _posY;
}

int InputManager::worldKeyboard(int key) {
    if(!_typeKeyboard)
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

InputManager& InputManager::getInstance() {
    return _instance;
}

bool InputManager::multipleKeysPressed(int number, ...) {
    // va_list ap;
    // int j;
    // bool val = true;
    // va_start(ap, number); // Requires the last fixed parameter (to get the address)
    // for(j = 0; j < number; j++) {
    //     int v = va_arg(ap, int);
    //     val &= glfwGetKey(window, v);
    // }
    // va_end(ap);
    // return val;
    return false;
}
void InputManager::getMousePosition(math::Vector2d& pos) {
    int tempX, tempY;
    SDL_GetMouseState(&tempX, &tempY);
    pos.x = tempX;
    pos.y = tempY;
}

bool InputManager::keyIsPressed(int key) {
    return _event.type == key;
}

bool InputManager::keyIsReleased(int key) {
    bool v = _keysReleased[key];
    _keysReleased[key] = false;
    return v;
}

int InputManager::getMouseButton(int id) {
    return _event.type == id;
}

void InputManager::processEvents() {
            _states = SDL_GetKeyboardState(nullptr);
        if(_event.type == SDL_WINDOWEVENT) {
            _closed = (_event.window.event == SDL_WINDOWEVENT_CLOSE);
        }
}

void InputManager::pollEvents() {
    _closed = false;
    while(SDL_PollEvent(&_event)) {
        processEvents();
    }
}

InputManager::~InputManager() {
}
