#include "Components/CDirectionalLight.h"

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