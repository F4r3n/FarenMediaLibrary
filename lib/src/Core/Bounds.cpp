#include "Core/Bounds.h"
#include <algorithm>
using namespace fm;

Bounds::Bounds() {
    center = fm::math::vec3(0, 0, 0);
    size = fm::math::vec3(0, 0, 0);
}

Bounds::Bounds(const fm::math::vec3& center, const fm::math::vec3& size) {
    this->center = center;
    this->size = size;
}

void Bounds::encapsulate(const Bounds& bounds) {
    size.x = std::max(size.x, bounds.size.x);
    size.y = std::max(size.y, bounds.size.y);
    size.z = std::max(size.z, bounds.size.z);

    center = (center + bounds.center) / 2.0f;
}
bool Bounds::intersects(const Bounds& bounds) {
    fm::math::vec3 ac = center;
    fm::math::vec3 bc = bounds.center;
    return (ac.x <= bc.x + bounds.size.x*bounds.scale.x*2.0f && ac.x + size.x*scale.x*2.0f >= bc.x) &&
           (ac.y <= bc.y + bounds.size.y*bounds.scale.y*2.0f && ac.y + size.y*scale.y*2.0f >= bc.y) &&
           (ac.z <= bc.z + bounds.size.z*bounds.scale.z*2.0f && ac.z + size.z*scale.z*2.0f >= bc.z);
}

void Bounds::setCenter(const fm::math::vec3& center) {
    this->center = center;
    
}
void Bounds::setSize(const fm::math::vec3& size) {
    this->size = size;
}

void Bounds::setScale(const fm::math::vec3& scale) {
    this->scale = scale;
}
