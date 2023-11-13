#include "Core/Bounds.h"
#include <algorithm>
#include <iostream>
using namespace fm;

Bounds::Bounds() {
    center = fm::math::vec3(0, 0, 0);
    size = fm::math::vec3(1, 1, 1);
    scale = fm::math::vec3(1,1,1);
}

Bounds::Bounds(const fm::math::vec3& center, const fm::math::vec3& size) {
    this->center = center;
    this->size = size;
    scale = fm::math::vec3(1,1,1);
}

void Bounds::encapsulate(const Bounds& bounds) {
    size.x = std::max(size.x, bounds.size.x);
    size.y = std::max(size.y, bounds.size.y);
    size.z = std::max(size.z, bounds.size.z);

    center = (center + bounds.center) / 2.0f;
}

fm::math::vec3 Bounds::getFinalSize() const {
    return size*scale;
}

bool Bounds::isInside(const fm::math::vec2 &point) {
     fm::math::vec3 ac = center;
     //std::cout << "THIS" << std::endl;
     //std::cout << center << std::endl;
     //std::cout << getFinalSize() << std::endl;
     //std::cout << "END THIS" << std::endl;
     //
     //std::cout << point << std::endl;
    //return true;
    return point.x < (ac.x + (0.5*getFinalSize().x)) && point.x > (ac.x - (0.5*getFinalSize().x)) &&
           point.y < (ac.y + (0.5*getFinalSize().y)) && point.y > (ac.y - (0.5*getFinalSize().y));
}

bool Bounds::isInside(const fm::math::vec3 &point) {
    fm::math::vec3 ac = center;
    return point.x < (ac.x + (0.5*getFinalSize().x)) && point.x > (ac.x - (0.5*getFinalSize().x)) &&
           point.y < (ac.y + (0.5*getFinalSize().y)) && point.y > (ac.y - (0.5*getFinalSize().y));
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

    return !((  bc.x >= ac.x + as.x)
            || (bc.x + bs.x <= ac.x)
            || (bc.y >= ac.y + as.y)
            || (bc.y + bs.y <= ac.y)
            || (bc.z >= ac.z + as.z)
            || (bc.z + bs.z <= ac.z)
    );
}

bool Bounds::intersects2D(const Bounds &bounds) {
    fm::math::vec3 as = this->size*this->scale;
    fm::math::vec3 bs = bounds.size*bounds.scale;
    
    fm::math::vec3 ac = center - as/2.0f;
    fm::math::vec3 bc = bounds.center - bs/2.0f;

    return !((  bc.x >= ac.x + as.x)
            || (bc.x + bs.x <= ac.x)
            || (bc.y >= ac.y + as.y)
            || (bc.y + bs.y <= ac.y)
    );
}


void Bounds::setCenter(const fm::math::vec3& inCenter) {
    this->center = inCenter;
    
}
void Bounds::setSize(const fm::math::vec3& inSize) {
    this->size = inSize;
}

void Bounds::setScale(const fm::math::vec3& inScale) {
    this->scale = inScale;
}
