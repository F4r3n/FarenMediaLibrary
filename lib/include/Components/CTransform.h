#pragma once
#include "component.h"
#include "Core/Math/Vector3.h"


class Entity;
class EntityManager;

namespace fmc {



class CTransform : public FMComponent<CTransform> {
    public:
        CTransform(const fm::math::Vector3f& position,
                   const fm::math::Vector3f& scale,
                   const fm::math::Vector3f& rotation, const int& layer = 1);
        CTransform();
        void Destroy();

        bool Serialize(json &ioJson) const override;
        bool Read(const json &inJSON) override;
        const std::string& GetName() const override;
        virtual size_t GetType() const {return kTransform;}


        void setFather(size_t id);
        void setFather(Entity* e);


        fm::math::Vector3f getWorldPos();
        fm::math::Vector3f getWorldPos(EntityManager& manager);

        fm::math::Vector3f position = { 0, 0, 0 };

        long idFather = -1;

        fm::math::Vector3f scale = { 1, 1, 1 };
        fm::math::Vector3f rotation = {0,0,0};
        int layer = 1;
        static const std::string name;


};
}
