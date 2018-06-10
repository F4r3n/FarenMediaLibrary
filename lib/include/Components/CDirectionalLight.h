#pragma once
#include <ECS.h>
#include "Core/Color.h"
#include "component.h"

namespace fmc {
class CDirectionalLight : public FMComponent<CDirectionalLight>{
    public:
        CDirectionalLight();
        CDirectionalLight(const fm::Color &color);
        ~CDirectionalLight();
        fm::Color color;
        static const std::string name;
        void Destroy()
        {
            EntityManager::get().removeComponent<CDirectionalLight>(BaseComponent::_IDEntity);
        }
};
}
