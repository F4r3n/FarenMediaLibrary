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
		BOX,
		SPHERE
	};

	class CBody3D : public FMComponent<CBody3D>
	{
	public:

		bool Serialize(json &ioJson) const override;
		bool Read(const json &inJSON) override;
		const std::string& GetName() const override;
		void Destroy() override;
		uint16_t GetType() const override { return kBody3D; }

		CBody3D(const fm::math::vec3 &inScale);
		CBody3D(float radius);
		CBody3D();

		~CBody3D();
		void SetMass(float inMass);
		void SetRadius(float radius);
		void SetScale(const fm::math::vec3 &inScale);

		float GetRadius() const { return _radius; }
		const fm::math::vec3& GetScale() const { return _scale; }
		SHAPE GetShape() const { return _shape; }
		float GetMass() const { return _mass; }

		void Init();
		void SetPosition(const fm::math::vec3 &inPosition);
		void SetRotation(const fm::math::vec3 &inRotation);

		void GetPosition(fm::math::vec3& outVec) const;
		void GetRotation(fm::math::vec3& outVec) const;


		btRigidBody* GetBody() const { return _body; }
		void AddToWorld(btDiscreteDynamicsWorld *inWorld);
		bool IsInit() const;

	private:

		btRigidBody* _body;
		float _mass;
		bool _isInWorld;
		fm::math::vec3 _scale;
		float _radius;
		SHAPE _shape;
	};
}