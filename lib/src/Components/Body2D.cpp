#include "Components/Body2D.h"
#include <Box2D/Box2D.h>

#include <Box2D/Common/b2Math.h>
#include <EntityManager.h>
using namespace fmc;
Body2D::Body2D(unsigned int w, unsigned int h, bool isDynamic) {
    this->isDynamic = isDynamic;
    size.x = w;
    size.y = h;
    _name = "Body2D";
}
Body2D::Body2D() {
    _name = "Body2D";
}

Body2D::~Body2D() {
}

void Body2D::Init(b2World* world, float P2M) 
{
	b2PolygonShape box;
	b2BodyDef bodyDef;
    if(isDynamic)
        bodyDef.type = b2_dynamicBody;
    body = world->CreateBody(&bodyDef);

    box.SetAsBox(size.x * P2M, size.y * P2M);

	b2FixtureDef fixtureDef;

    fixtureDef.shape = &box;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
	body->CreateFixture(&fixtureDef);
    body->SetUserData(this);
    isReady = true;
}

const std::string Body2D::name = "Body2D";
void Body2D::StartContact() {
    number_contact++;
}

void Body2D::EndContact() {
    number_contact--;
}

void Body2D::ApplyForceCenter(fm::math::Vector2f power) {
    body->ApplyForce({power.x, power.y}, body->GetWorldCenter(), true);
}

void Body2D::ApplyForceCenter2(float x, float y) {
    body->ApplyForce(b2Vec2(x, y), body->GetWorldCenter(), true);
}

void Body2D::ApplyForce(fm::math::Vector2f &&power, fm::math::Vector2f &&pos) {
    body->ApplyForce({power.x, power.y}, {pos.x, pos.y}, true);
   
}

void Body2D::SetFriction(float value) {
	body->GetFixtureList()[0].SetFriction(value);
}

void Body2D::Destroy()
{
    EntityManager::get().removeComponent<Body2D>(BaseComponent::_IDEntity);

}
