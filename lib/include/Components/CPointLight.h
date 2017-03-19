#pragma once
#include "Component.h"
#include "Core/Color.h"
#include "Core/Math/Vector2.h"
namespace fmc {
class CPointLight : public Component<CPointLight> {
public:
    fm::Color color;
    float radius = 100;
};
}