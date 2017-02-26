#pragma once
#include "InputManager.h"
#include <tuple>
namespace fm
{
class Input
{
public:
    static bool keyIsPressed(int key);
    static bool keyIsReleased(int key);
    static void getMousePosition(Vector2<double>& pos);
    static void getMousePosition(double& posX, double& posY);
    static double getMousePositionX();
    static double getMousePositionY();
    static std::tuple<double, double> getMousePosition();
    static Vector2d getMousePositionVector();
    static int getMouseButton(int id);
};
}