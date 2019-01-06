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

		CBody3D(float w, float h, float l);
		~CBody3D();
		void SetMass(float inMass);
		void Init();
		void SetPosition(const fm::math::vec3 &inPosition);
		void SetRotation(const fm::math::vec3 &inRotation);

		void GetPosition(fm::math::vec3& outVec) const;
		void GetRotation(fm::math::vec3& outVec) const;

		btRigidBody* GetBody() const { return _body; }
		void AddToWorld(btDiscreteDynamicsWorld *inWorld);
		bool IsInit() const;

	private:

		btCollisionShape* _shape;
		btRigidBody* _body;
		SHAPE _type;
		float _mass;
		bool _isInWorld;
	};
}