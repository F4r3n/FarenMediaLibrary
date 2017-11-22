#pragma once
#include <Component.h>
#include "Core/Math/Vector2.h"
#include "EntityManager.h"

//#include "Core/MacroHelper.h"

class Entity;


namespace fmc {
class CTransform : public Component<CTransform> {
public:
    CTransform(const fm::math::Vector2f& position, const fm::math::Vector2f& scale, const float& rotation, const int& layer = 1);
    CTransform();
    ~CTransform() {
    }

    void setFather(size_t id) {
        this->idFather = id;
    }

    void setFather(Entity* e);

    fm::math::Vector2f getWorldPos();
    fm::math::Vector2f getWorldPos(EntityManager& manager);

    fm::math::Vector2f position = { 0, 0 };

    size_t idFather = std::numeric_limits<size_t>::max();

    fm::math::Vector2f scale = { 100, 100 };
    float rotation = 0;
    int layer = 1;
    static const std::string name;


};
}
