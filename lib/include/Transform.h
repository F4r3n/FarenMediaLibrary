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
            position[0] = x;
            position[1] = y;
        }
        void setPosition(float x, float y)
        {
            position[0] = x;
            position[1] = y;
        }
        Vector2f getPositionV()
        {
            return Vector2f(position[0], position[1]);
        }
        float* getPosition()
        {
            return position;
        }
        void setPositionX(float x) {
            position[0] = x;
        } 
        void setPositionY(float y) {
            position[1] = y;
        } 
        float getPositionX()
        {
            return position[0];
        }
        float getPositionY()
        {
            return position[1];
        }

        void setScale(float x, float y)
        {
            scale[0] = x;
            scale[1] = y;
        }
        float getScaleX()
        {
            return scale[0];
        }
        float getScaleY()
        {
            return scale[1];
        }
        float* getScale()
        {
            return scale;
        }
        Vector2f getScaleV()
        {
            return Vector2f(scale[0], scale[1]);
        }
        
        float position[2];
        float scale[2];
        float rotation;
    };
}