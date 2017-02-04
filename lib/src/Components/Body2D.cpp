#include "Components/Body2D.h"
#include <iostream>
using namespace fmc;


const std::string Body2D::name = "Body2D";
void Body2D::startContact() {
    number_contact++;
}

void Body2D::endContact() {
    number_contact--;
}

void Body2D::applyForceCenter(fm::Vector2f power) {
    body->ApplyForce((b2Vec2)power, body->GetWorldCenter(), true);
}

void Body2D::applyForceCenter2(float x, float y) {
    body->ApplyForce(b2Vec2(x, y), body->GetWorldCenter(), true);
}

void Body2D::applyForce(fm::Vector2f &&power, fm::Vector2f &&pos) {
    body->ApplyForce((b2Vec2)power, (b2Vec2)pos, true);
   
}