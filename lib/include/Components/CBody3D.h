#pragma once
#include "component.h"
#include "Core/Math/Vector3.h"

class btCollisionShape;
class btRigidBody;
class btDiscreteDynamicsWorld;
namespace fmc
{
	enum SHAPE
	{
		BOX
	};

	class CBody3D : public FMComponent<CBody3D>
	{
	public:


		const std::string& GetName() const override;
		void Destroy() override;

		uint16_t GetType() const override { return kBody3D; }

		CBody3D(const fm::math::vec3 &inVec3);
		~CBody3D();
		void SetMass(float inMass);
		void Init();
		void SetPosition(const fm::math::vec3 &inPosition);
		void SetRotation(const fm::math::vec3 &inRotation);
		void SetScale(const fm::math::vec3 &inScale);

		void GetPosition(fm::math::vec3& outVec) const;
		void GetRotation(fm::math::vec3& outVec) const;


		btRigidBody* GetBody() const { return _body; }
		void AddToWorld(btDiscreteDynamicsWorld *inWorld);
		bool IsInit() const;

	private:
		void _InitShape(const fm::math::vec3 &inScale);

		btCollisionShape* _shape;
		btRigidBody* _body;
		SHAPE _type;
		float _mass;
		bool _isInWorld;
	};
}