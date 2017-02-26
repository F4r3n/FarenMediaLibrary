#include "Input/Input.h"

using namespace fm;
bool Input::keyIsPressed(int key) {
    return InputManager::getInstance().keyIsPressed(key);
}

bool Input::keyIsReleased(int key) {
    return InputManager::getInstance().keyIsReleased(key);
}

int Input::getMouseButton(int id) {
    return InputManager::getInstance().getMouseButton(id);
}
void Input::getMousePosition(Vector2<double>& pos) {
    InputManager::getInstance().getMousePosition(pos);
}

void Input::getMousePosition(double& x, double& y) {
    InputManager::getInstance().getMousePosition(x, y);
}

double Input::getMousePositionX() {
    return InputManager::getMousePositionX();
}

double Input::getMousePositionY() {
    return InputManager::getMousePositionY();
}

std::tuple<double, double> Input::getMousePosition() {
    double x, y;
    InputManager::getInstance().getMousePosition(x, y);
    return std::make_tuple(x, y);
}

Vector2d Input::getMousePositionVector() {
    Vector2d pos;
    InputManager::getInstance().getMousePosition(pos);
    return pos;
}