#include "Components/Body2D.h"
#include <Box2D/Common/b2Math.h>
#include <iostream>
using namespace fmc;


const std::string Body2D::name = "Body2D";
void Body2D::startContact() {
    number_contact++;
}

void Body2D::endContact() {
    number_contact--;
}

void Body2D::applyForceCenter(fm::math::Vector2f power) {
    body->ApplyForce({power.x, power.y}, body->GetWorldCenter(), true);
}

void Body2D::applyForceCenter2(float x, float y) {
    body->ApplyForce(b2Vec2(x, y), body->GetWorldCenter(), true);
}

void Body2D::applyForce(fm::math::Vector2f &&power, fm::math::Vector2f &&pos) {
    body->ApplyForce({power.x, power.y}, {pos.x, pos.y}, true);
   
}

void Body2D::SetFriction(float value) {
    fixture->SetFriction(value);
}
