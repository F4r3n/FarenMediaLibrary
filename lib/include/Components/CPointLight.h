#pragma once
#include "component.h"
#include "Core/Color.h"
#include "Core/Math/Vector2.h"
namespace fmc {
class CPointLight : public FMComponent<CPointLight> {
    public:
        CPointLight();

        fm::Color color;
        float radius = 100;
};
}
