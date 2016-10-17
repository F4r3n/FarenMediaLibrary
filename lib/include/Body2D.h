#pragma once
#include "Component.h"
#include <Box2D/Box2D.h>
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
    
    void init(b2World* world) {
        if(isDynamic)
            bodyDef.type = b2_dynamicBody;
        body = world->CreateBody(&bodyDef);
        box.SetAsBox(width, height);
        fixture.shape = &box;
        fixture.density = density;
        fixture.friction = friction;
        body->CreateFixture(&fixture);
    }

    b2BodyDef bodyDef;
    b2Body* body;
    b2PolygonShape box;
    b2FixtureDef fixture;
    
    float density = 0.0f;
    float friction = 0.0f;
    
    private:
    bool isDynamic = false;
    unsigned int width;
    unsigned int height;
};
}