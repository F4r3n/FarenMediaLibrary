#pragma once
#include "Core/Color.h"
#include "component.h"

namespace fmc {
class CDirectionalLight
{
    public:
        CDirectionalLight();
        CDirectionalLight(const fm::Color &color);
        ~CDirectionalLight();
        fm::Color color;
        static const std::string name;
private:
	std::string _name;
};
}
