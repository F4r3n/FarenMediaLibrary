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
    std::cout << "Apply power " << power.x << std::endl;
    body->ApplyForce((b2Vec2)power, body->GetWorldCenter(), true);
}

void Body2D::applyForceCenter2(float x, float y) {
    std::cout << "Apply power2 " << x << std::endl;
    body->ApplyForce(b2Vec2(x, y), body->GetWorldCenter(), true);
    //body->SetTransform(b2Vec2(x, y), 0);
}

void Body2D::applyForce(fm::Vector2f &&power, fm::Vector2f &&pos) {
    body->ApplyForce((b2Vec2)power, (b2Vec2)pos, true);
   
}