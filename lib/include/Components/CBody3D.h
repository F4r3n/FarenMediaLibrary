#pragma once
#include "component.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Quaternion.h"

class btCollisionShape;
class btRigidBody;
class btDiscreteDynamicsWorld;
class btGhostObject;
class btTransform;
namespace fmc
{
	class CCollider;
}
namespace fmc
{

	class CBody3D : public FMComponent<CBody3D>
	{
	public:

		bool Serialize(json &ioJson) const override;
		bool Read(const json &inJSON) override;
		const std::string& GetName() const override;
		void Destroy() override;
		uint16_t GetType() const override { return kBody3D; }

		CBody3D();

		~CBody3D();
		void SetMass(float inMass);

		float GetMass() const { return _mass; }

		void Init(CCollider *inCollider);
		void SetPosition(const fm::math::vec3 &inPosition);
		void SetRotation(const fm::math::Quaternion &inRotation);

		void GetPosition(fm::math::vec3& outVec) const;
		void GetRotation(fm::math::Quaternion &outQuaternion) const;


		btRigidBody* GetBody() const { return _body; }
		btGhostObject* GetGhost() const { return _ghostObject; }
		void AddToWorld(btDiscreteDynamicsWorld *inWorld);
		bool IsInit() const;

		bool SetGhost(bool value);
		bool IsGhost() const;
	private:
		void _GetCurrentTransform(btTransform& transform)const;
		void _Init();
		btRigidBody* _body;
		float _mass;
		bool _isInWorld;
		btGhostObject* _ghostObject;
		bool _isGhost;

	};
}