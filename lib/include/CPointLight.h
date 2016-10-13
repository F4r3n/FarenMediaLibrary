#pragma once
#include "Component.h"
#include "Color.h"
#include "Vector2.h"
namespace fmc {
class CPointLight : public Component<CPointLight> {
public:
    fm::Color color;
    float radius = 100;
};
}