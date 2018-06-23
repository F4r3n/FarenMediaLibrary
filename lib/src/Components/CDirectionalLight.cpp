#include "Components/CDirectionalLight.h"
#include <ECS.h>

using namespace fmc;
const std::string CDirectionalLight::name = "Directional Light";

CDirectionalLight::CDirectionalLight() {
    _name = "DirectionalLight";
}
CDirectionalLight::CDirectionalLight(const fm::Color& color) {
    this->color = color;
}
CDirectionalLight::~CDirectionalLight() {
    
}

void CDirectionalLight::Destroy()
{
    EntityManager::get().removeComponent<CDirectionalLight>(BaseComponent::_IDEntity);
}
