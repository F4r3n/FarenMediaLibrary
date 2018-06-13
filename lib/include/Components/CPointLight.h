#pragma once
#include "component.h"
#include "Core/Color.h"
#include "Core/Math/Vector2.h"
namespace fmc {
class CPointLight : public FMComponent<CPointLight> {
    public:
        CPointLight();
        virtual size_t GetType() const {return kPointLight;}

        fm::Color color;
        float radius = 100;
        void Destroy()
        {
            EntityManager::get().removeComponent<CPointLight>(BaseComponent::_IDEntity);
        }
};
}
