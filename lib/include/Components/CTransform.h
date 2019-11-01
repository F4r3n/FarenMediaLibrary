#pragma once
#include "component.h"
#include "Core/Transform.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Matrix.h"

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
        virtual uint16_t GetType() const {return kTransform;}
		fm::Transform GetTransform() const;
		fm::math::mat GetLocalMatrixModel() const;
		void SetLocalMatrixModel(const fm::math::mat &inLocalMatrix);
		void From(const fmc::CTransform *inTransform);

        void setFather(ecs::id id);
        void setFather(Entity* e);


        fm::math::Vector3f getWorldPos() const;
        fm::math::Vector3f getWorldPos(EntityManager& manager) const;

        long idFather = -1;


        int layer = 1;
        static const std::string name;

		fm::math::Vector3f position = { 0, 0, 0 };
		fm::math::Vector3f scale = { 1, 1, 1 };
		fm::math::Vector3f rotation = { 0,0,0 };
};
}
