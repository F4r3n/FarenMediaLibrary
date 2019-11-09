#pragma once
#include "component.h"
#include "Core/Transform.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Quaternion.h"
class Entity;
class EntityManager;



namespace fmc {



class CTransform : public FMComponent<CTransform> {
    public:
        CTransform(const fm::math::Vector3f& position,
                   const fm::math::Vector3f& scale,
                   const fm::math::Vector3f& rotation, const int& layer = 1);
        CTransform();
		~CTransform();
        void				Destroy();

        bool				Serialize(json &ioJson) const override;
        bool				Read(const json &inJSON) override;
        const std::string&	GetName() const override;
        virtual uint16_t	GetType() const {return kTransform;}
		fm::Transform		GetTransform() const;
		fm::math::mat		GetLocalMatrixModel(bool opposePosition = false) const;
		fm::math::mat		GetWorldMatrix(bool opposePosition = false) const;
		void				SetLocalMatrixModel(const fm::math::mat &inLocalMatrix);
		void				From(const fmc::CTransform *inTransform);

        void				setFather(ecs::id id);
        void				setFather(Entity* e);
		void				RemoveFather();

        ecs::id idFather;


        int layer = 1;
        static const std::string name;

		fm::math::vec3 position;
		fm::math::vec3 scale;
		fm::math::Quaternion rotation;//Quaternion
private:
		bool _hasFather;
};
}
