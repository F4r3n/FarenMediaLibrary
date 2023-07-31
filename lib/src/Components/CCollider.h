#pragma once
#include "component.h"
#include "Core/Math/Vector3.h"
#include <nlohmann/json_fwd.hpp>
class btGhostObject;
class btCollisionShape;
class btDynamicsWorld;

namespace fm
{
	struct Transform;
}

namespace fmc
{
	class CCollider : public FMComponent<CCollider>
	{
	public:
		enum class SHAPE
		{
			BOX,
			BOX2D,
			SPHERE,
			LAST
		};

		CCollider();
		~CCollider();
		bool Serialize(nlohmann::json &ioJson) const override;
		bool Read(const nlohmann::json &inJSON) override;
		const std::string& GetName() const override;
		uint16_t GetType() const override { return kCollider; }

		void Init(const fm::math::vec3& inScale);
		bool IsInit();

		btCollisionShape* GetCollisionShape() const { return _collisionShape; }
		bool SetShape(SHAPE inShape);
		CCollider::SHAPE GetShape() const;

		const fm::math::vec3  GetScale() const;
		void SetScale(const fm::math::vec3 &inScale);
	private:
		SHAPE _shape;
		fm::math::vec3 _scale;

		btCollisionShape *_collisionShape;
		//TODO Position/Rotation relative to the entity

	};
}