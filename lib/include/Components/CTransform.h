#pragma once
#include <Component.h>
#include "Vector2.h"
#include "EntityManager.h"

#include "Serializer.h"
class Entity;

namespace fmc {
class CTransform : public Component<CTransform>, public Serializer {
public:
    CTransform(const fm::Vector2f& position, const fm::Vector2f& scale, const float& rotation, const int& layer = 1);
    CTransform();
    ~CTransform() {
    }

    void setFather(size_t id) {
        this->idFather = id;
    }

    void setFather(Entity* e);

    fm::Vector2f getWorldPos();
    fm::Vector2f getWorldPos(EntityManager& manager);

    fm::Vector2f position = { 0, 0 };

    size_t idFather = std::numeric_limits<size_t>::max();

    fm::Vector2f scale = { 100, 100 };
    float rotation = 0;
    int layer = 1;
    static const std::string name;

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
    void parse(rapidjson::Value &value) override;
     
};
}