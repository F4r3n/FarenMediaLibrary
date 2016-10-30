#pragma once
#include <ECS.h>
#include "Color.h"

namespace fmc {
class CDirectionalLight : public Component<CDirectionalLight>{
public:
    CDirectionalLight();
    CDirectionalLight(const fm::Color &color);
    ~CDirectionalLight();
    fm::Color color;
    static const std::string name;
};
}