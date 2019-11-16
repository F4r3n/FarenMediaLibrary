#pragma once
#include "component.h"
#include "Core/Math/Vector3.h"
#include "Core/Transform.h"
class btGhostObject;
class btCollisionShape;
class btDynamicsWorld;
namespace fmc
{

	enum SHAPE
	{
		BOX,
		SPHERE
	};

	class CCollider : public FMComponent<CCollider>
	{
	public:

		CCollider();
		~CCollider();
		bool Serialize(json &ioJson) const override;
		bool Read(const json &inJSON) override;
		const std::string& GetName() const override;
		void Destroy() override;
		uint16_t GetType() const override { return kCollider; }

		void Init(const fm::Transform& inTransform);
		bool IsInit();

		btCollisionShape* GetCollisionShape() const { return _collisionShape; }
		bool SetShape(SHAPE inShape);
		SHAPE GetShape() const;

		const fm::math::vec3  GetScale() const;
		void SetScale(const fm::math::vec3 &inScale);
	private:
		SHAPE _shape;
		fm::math::vec3 _scale;

		btCollisionShape *_collisionShape;
		bool _isInWorld;
		//TODO Position/Rotation relative to the entity

	};
}