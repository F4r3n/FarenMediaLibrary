#pragma once
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector2.h"

namespace fm {
class Bounds {
public:
    Bounds();
    Bounds(const fm::math::vec3& center, const fm::math::vec3& size);
    void encapsulate(const Bounds& bounds);
    bool isInside(const fm::math::vec3 &point);
    bool isInside(const fm::math::vec2 &point);

    bool intersects2D(const Bounds &bounds);
    bool intersects(const Bounds &bounds);
    void setCenter(const fm::math::vec3 &center);
    void setSize(const fm::math::vec3 &size);
    void setScale(const fm::math::vec3 &scale);
    
    fm::math::vec3 getSize() const;
    fm::math::vec3 getCenter() const;
    fm::math::vec3 getScale() const;
    fm::math::vec3 getFinalSize() const;
    
        fm::math::vec3 center;
private:

    fm::math::vec3 size;
    fm::math::vec3 scale;
};
}