#include "Components/CBody2D.h"
#include <Box2D/Box2D.h>

#include <Box2D/Common/b2Math.h>
#include <EntityManager.h>
#include "Components/CCollider2D.h"

using namespace fmc;
CBody2D::CBody2D(unsigned int w, unsigned int h, bool isDynamic)
{
    _isDynamic = isDynamic;
    _name = "Body2D";
}
CBody2D::CBody2D()
{
    _name = "Body2D";
}

CBody2D::~CBody2D()
{
}

void CBody2D::Init(fmc::CCollider2D* inCollider, b2World* world)
{
	b2PolygonShape box;
	b2BodyDef bodyDef;
    if(_isDynamic)
        bodyDef.type = b2_dynamicBody;
    _body = world->CreateBody(&bodyDef);


	b2FixtureDef fixtureDef;

    fixtureDef.shape = inCollider->GetCollisionShape();
    fixtureDef.density = _density;
    fixtureDef.friction = _friction;
	_body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
}



void CBody2D::ApplyForceCenter(fm::math::Vector2f power) {
    _body->ApplyForce({power.x, power.y}, _body->GetWorldCenter(), true);
}

void CBody2D::ApplyForceCenter2(float x, float y) {
    _body->ApplyForce(b2Vec2(x, y), _body->GetWorldCenter(), true);
}

void CBody2D::ApplyForce(fm::math::Vector2f &&power, fm::math::Vector2f &&pos) {
    _body->ApplyForce({power.x, power.y}, {pos.x, pos.y}, true);
   
}

void CBody2D::SetFriction(float value) {
	_body->GetFixtureList()[0].SetFriction(value);
}

void CBody2D::Destroy()
{
    EntityManager::get().removeComponent<CBody2D>(BaseComponent::_IDEntity);

}
