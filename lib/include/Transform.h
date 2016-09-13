#pragma once
#include "Vector2.h"
namespace fm
{
    class Transform
    {
    public:
    Transform() {}
        Transform(float x, float y)
        {
            position.x = x;
            position.y = y;
        }
       
        Vector2f position;
        Vector2f scale;
    };
}