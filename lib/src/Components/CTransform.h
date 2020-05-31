#pragma once
#include "component.h"
#include "Core/Transform.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Quaternion.h"
#include <nlohmann/json_fwd.hpp>
class Entity;
class EntityManager;



namespace fmc {



class CTransform : public FMComponent<CTransform>
{
    public:
        CTransform(const fm::math::Vector3f& position,
                   const fm::math::Vector3f& scale,
                   const fm::math::Vector3f& rotation);
        CTransform();
		~CTransform();
        void				Destroy();

        bool						Serialize(nlohmann::json &ioJson) const override;
        bool						Read(const nlohmann::json &inJSON) override;
        const std::string&			GetName() const override;
        virtual uint16_t			GetType() const {return kTransform;}
		fm::Transform				GetTransform() const;
		fm::math::mat				GetLocalMatrixModel(bool opposePosition = false) const;
		void						From(const fmc::CTransform *inTransform);

        void						setFather(ecs::id id);
        void						setFather(Entity* e);
		void						RemoveFather();

		void						SetRotation(const fm::math::Quaternion& inQuaternion);
		void						SetScale(const fm::math::vec3& inScale);
		void						SetPosition(const fm::math::vec3& inPosition);

		void						SetWorldRotation(const fm::math::Quaternion& inQuaternion);
		void						SetWorldPosition(const fm::math::vec3& inPosition);
		void						SetWorldScale(const fm::math::vec3& inScale);

		const fm::math::vec3&		GetPosition() const;
		const fm::math::Quaternion& GetRotation() const;
		const fm::math::vec3&		GetScale() const;


		fm::math::vec3				GetWorldPosition() const;
		const fm::math::Quaternion& GetWorldRotation() const;
		fm::math::vec3				GetWorldScale() const;

		const fm::math::mat&		GetWorldPosMatrix() const;

		
		CTransform*					GetFather() const;

		static fm::math::mat CreateMatrixModel(const fm::math::vec3& pos, const fm::math::vec3& scale, const fm::math::Quaternion &q, bool ortho = true);

private:
	void _UpdateWorldMatrix() const;
private:
	ecs::id							_idFather;

	fm::math::vec3					_position;
	fm::math::vec3					_scale;
	fm::math::Quaternion			_rotation;

	mutable fm::math::Quaternion	_worldRotation;
	mutable fm::math::mat			_worldPosition;

	mutable bool					_isDirty = false;
	bool							_hasFather;
};
}
