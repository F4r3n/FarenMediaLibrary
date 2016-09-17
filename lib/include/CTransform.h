#pragma once
#include <Component.h>
#include "Vector2.h"
namespace fmc
{
class CTransform : public Component<CTransform>
{
    public:
    fm::Vector2f position = {0,0};
    fm::Vector2f scale = {100,100};
    float rotation = 0;
};
}