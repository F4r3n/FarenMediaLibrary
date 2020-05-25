#pragma once
#include "component.h"
#include "../Core/Math/Vector2.h"
#include <string>
#include <memory>

class b2Body;
class b2PolygonShape;
class b2Fixture;
class b2World;
namespace fmc
{
	class CCollider2D;
}
namespace fmc 
{
	
class CBody2D : public FMComponent<CBody2D> {
   public:
	   CBody2D(unsigned int w, unsigned int h, bool isDynamic = false);
	   CBody2D();

    ~CBody2D();

    void Init(CCollider2D* inCollider, b2World* world);
    void Destroy() override;

    void ApplyForceCenter(fm::math::Vector2f power);
    void ApplyForce(fm::math::Vector2f&& power, fm::math::Vector2f&& pos);
    void ApplyForceCenter2(float x, float y);

	void SetDensity(float inValue);
	float GetDensity() const;

	void SetFriction(float inValue);
	float GetFriction() const;

	b2Body* GetBody() const { return _body; }

private:

	float _density = 0.0f;
	float _friction = 0.0f;
	bool _isDynamic = false;

	b2Body* _body = nullptr;

};
}
