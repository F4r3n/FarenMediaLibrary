#include "Components/CPointLight.h"
#include <EntityManager.h>
using namespace fmc;
CPointLight::CPointLight()
{
    _name = "PointLight";
}

void CPointLight::Destroy()
{
    EntityManager::get().removeComponent<CPointLight>(BaseComponent::_IDEntity);
}

