#pragma once
#include <ECS.h>
#include "Core/Color.h"

namespace fmc {
class CDirectionalLight : public Component<CDirectionalLight>{
public:
    CDirectionalLight();
    CDirectionalLight(const fm::Color &color);
    ~CDirectionalLight();
    fm::Color color;
    static const std::string name;
    
    int* get(int v) {return nullptr;}

};
}