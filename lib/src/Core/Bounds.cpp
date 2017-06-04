#include "Core/Bounds.h"
#include <algorithm>
#include <iostream>
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

bool Bounds::isInside(const fm::math::vec3 &point) {
    fm::math::vec3 ac = center;
    return true;
    //return (ac.x <= point.x + bounds.size.x*bounds.scale.x*2.0f && ac.x + size.x*scale.x*2.0f >= point.x) &&
    //       (ac.y <= point.y + bounds.size.y*bounds.scale.y*2.0f && ac.y + size.y*scale.y*2.0f >= point.y) &&
    //       (ac.z <= point.z + bounds.size.z*bounds.scale.z*2.0f && ac.z + size.z*scale.z*2.0f >= point.z);
}

fm::math::vec3 Bounds::getSize() const {
    return size;
}
fm::math::vec3 Bounds::getCenter() const {
    return center;
}
fm::math::vec3 Bounds::getScale() const{
    return scale;
}

bool Bounds::intersects(const Bounds &bounds) {
    fm::math::vec3 as = this->size*this->scale;
    fm::math::vec3 bs = bounds.size*bounds.scale;
    
    fm::math::vec3 ac = center - as/2.0f;
    fm::math::vec3 bc = bounds.center - bs/2.0f;
    //std::cout << bc << " " << bs << std::endl;
    //std::cout << ac << " " << as << std::endl;
    //std::cout << !((bc.x >= ac.x + as.x)
    //        || (bc.x + bs.x <= ac.x)
    //        || (bc.y >= ac.y + as.y)
    //        || (bc.y + bs.y <= ac.y)
    //) << " \n " << std::endl;
    return !((  bc.x >= ac.x + as.x)
            || (bc.x + bs.x <= ac.x)
            || (bc.y >= ac.y + as.y)
            || (bc.y + bs.y <= ac.y)
            || (bc.z >= ac.z + as.z)
            || (bc.z + bs.z <= ac.z)
    );
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
