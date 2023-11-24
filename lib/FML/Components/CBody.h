#pragma once
#include "component.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Quaternion.h"
#include "Components/cevent.hpp"
#include "nlohmann/json_fwd.hpp"
#include <entt/entt.hpp>
class btCollisionShape;
class btRigidBody;
class btDiscreteDynamicsWorld;
class btGhostObject;
class btTransform;
class btCollisionObject;

namespace fmc
{
	class CTransform;
	class CCollider;
}

namespace fm
{
	class CollisionEvent : public BaseEvent
	{
	public:
		CollisionEvent() {}
		CollisionEvent(entt::handle h, const fm::math::vec3 &inTouchPoint, const fm::math::vec3 &inNormalPoint)
		{
			_other = h;
			_touchPoint = inTouchPoint;
			_normalPoint = inNormalPoint;
		}
		~CollisionEvent() {}
		size_t GetType() const {return EventKind::COLLISION;}
		entt::handle GetEntity() const { return _other; }
		const fm::math::vec3& GetTouchPoint()const { return _touchPoint; }
		const fm::math::vec3& GetNormalPoint()const { return _normalPoint; }

	private:
		entt::handle _other;
		fm::math::vec3 _touchPoint;
		fm::math::vec3 _normalPoint;
	};
}


namespace fmc
{

	class CBody
	{
	public:

		bool Serialize(nlohmann::json &ioJson) const;
		bool Read(const nlohmann::json &inJSON);
		const std::string& GetName() const;
		uint16_t GetType() const { return kBody; }

		CBody();
		~CBody();

		void Init(CCollider *inCollider, const fmc::CTransform& inTransform, entt::handle inHandle);
		void SetPosition(const fm::math::vec3 &inPosition);
		void SetRotation(const fm::math::Quaternion &inRotation);

		void GetPosition(fm::math::vec3& outVec) const;
		void GetRotation(fm::math::Quaternion &outQuaternion) const;

		void AddToWorld(btDiscreteDynamicsWorld *inWorld);
		bool IsInit() const;

		bool SetGhost(bool value);
		bool IsGhost() const;

		void SetMass(float inMass, fmc::CCollider* inCollider);

		float GetMass() const;

		void SetGravity(const fm::math::vec3& inAcceleration);
		const fm::math::vec3& GetGravity() const;

		void SetLinearVelocity(const fm::math::vec3& inAcceleration);
		fm::math::vec3 GetLinearVelocity() const;

		void SetFriction(float inFriction);
		float GetFriction() const;

		void SetRestitution(float inMass);
		float GetRestitution() const;

		void SetLinearFactor(const fm::math::vec3& inFactor);
		const fm::math::vec3& GetLinearFactor() const;

		void SetAngularFactor(const fm::math::vec3& inFactor);
		const fm::math::vec3& GetAngularFactor() const;

		entt::handle GetEntity() const { return _entity; }
	private:
		btRigidBody*	_GetBody() const;
		btGhostObject*	_GetGhost() const;
		void			_GetCurrentTransform(btTransform& transform)const;
		void			_Init();

	private:
		btCollisionObject*		_body;
		entt::handle			_entity;

		mutable fm::math::vec3	_gravity;
		mutable float			_mass;
		mutable fm::math::vec3	_angularFactor;
		mutable fm::math::vec3	_linearFactor;
		mutable float			_friction;
		mutable	float			_restitution;

		bool					_isInWorld;
		bool					_isGhost;
	private:
		std::string _name;

	};
}