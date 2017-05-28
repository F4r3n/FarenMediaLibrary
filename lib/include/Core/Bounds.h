#pragma once
#include "Core/Math/Vector3.h"
namespace fm {
class Bounds {
public:
    Bounds();
    Bounds(const fm::math::vec3& center, const fm::math::vec3& size);
    void encapsulate(const Bounds& bounds);
    bool intersects(const Bounds& bounds);
    void setCenter(const fm::math::vec3 &center);
    void setSize(const fm::math::vec3 &size);
    void setScale(const fm::math::vec3 &scale);
private:
    fm::math::vec3 center;
    fm::math::vec3 size;
    fm::math::vec3 scale;
};
}