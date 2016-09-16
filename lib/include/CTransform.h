#pragma once
#include <Component.h>
#include "Vector2.h"
namespace fmc
{
class CTransform : public Component<CTransform>
{
    public:
    fm::Vector2f position;
    fm::Vector2f scale;
    float rotation;
};
}