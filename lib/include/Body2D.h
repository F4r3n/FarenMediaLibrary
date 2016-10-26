#pragma once
#include "Component.h"
#include <Box2D/Box2D.h>
#include "Vector2.h"
namespace fmc {

class Body2D : public Component<Body2D> {
public:
    Body2D(unsigned int w, unsigned int h, bool isDynamic = false) {
        this->isDynamic = isDynamic;
        this->width = w;
        this->height = h;
    }
    ~Body2D() {
    }

    void init(b2World* world, float P2M) {
        if(isDynamic)
            bodyDef.type = b2_dynamicBody;
        body = world->CreateBody(&bodyDef);
        box.SetAsBox(width*P2M, height*P2M);
        fixture.shape = &box;
        fixture.density = density;
        fixture.friction = friction;
        body->CreateFixture(&fixture);
        body->SetUserData(this);
    }
    
    void startContact();
    void endContact();
    
    void applyForceCenter(fm::Vector2f power);
    void applyForce(fm::Vector2f &&power, fm::Vector2f &&pos);
    void applyForceCenter2(float x, float y);
    b2BodyDef bodyDef;
    b2Body* body;
    b2PolygonShape box;
    b2FixtureDef fixture;

    float density = 0.0f;
    float friction = 0.0f;
    int number_contact = 0;
    size_t *identity = nullptr;
    
private:
    bool isDynamic = false;
    unsigned int width;
    unsigned int height;
};


}