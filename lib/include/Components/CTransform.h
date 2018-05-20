#pragma once
#include "component.h"
#include "Core/Math/Vector2.h"
#include "EntityManager.h"


class Entity;


namespace fmc {



class CTransform : public FMComponent<CTransform> {
    public:
        CTransform(const fm::math::Vector2f& position, const fm::math::Vector2f& scale, const float& rotation, const int& layer = 1);
        CTransform();

        bool Serialize(json &ioJson) const override;
        bool Read(const json &inJSON) override;
        const std::string& GetName() const override;
        virtual size_t GetType() const {return kTransform;}


        void setFather(size_t id);
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
