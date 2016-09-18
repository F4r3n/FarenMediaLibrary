#pragma once
#include <Component.h>
#include "Vector2.h"
namespace fmc
{
class CTransform : public Component<CTransform>
{
    public:
    CTransform(const fm::Vector2f &position, const fm::Vector2f &scale, const float &rotation); 
    CTransform();
    fm::Vector2f position = {0,0};
    fm::Vector2f scale = {100,100};
    float rotation = 0;
};
}