#pragma once
#include "Component.h"
#include <Box2D/Box2D.h>
#include "../Core/Math/Vector2.h"
#include <string>
#include "Serializer.h"
namespace fmc {

class Body2D : public Component<Body2D>, public Serializer {
public:
    Body2D(unsigned int w, unsigned int h, bool isDynamic = false) {
        this->isDynamic = isDynamic;
        size.x = w;
        size.y = h;
    }
    Body2D() {
    }

    ~Body2D() {
    }

    void init(b2World* world, float P2M) {
        if(isDynamic)
            bodyDef.type = b2_dynamicBody;
        body = world->CreateBody(&bodyDef);
        box.SetAsBox(size.x * P2M, size.y * P2M);
        fixtureDef.shape = &box;
        fixtureDef.density = density;
        fixtureDef.friction = friction;
        fixture = body->CreateFixture(&fixtureDef);
        body->SetUserData(this);
        isReady = true;
    }

    void startContact();
    void endContact();
    void SetFriction(float friction);

    void applyForceCenter(fm::math::Vector2f power);
    void applyForce(fm::math::Vector2f&& power, fm::math::Vector2f&& pos);
    void applyForceCenter2(float x, float y);

    float density = 0.0f;
    float friction = 0.0f;
    int number_contact = 0;
    size_t* identity = nullptr;
    fm::math::Vector2f size;
    bool isDynamic = false;
    
    static const std::string name;
    
    bool isReady = false;
void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override{}
    void parse(rapidjson::Value &value) override {}
            b2BodyDef bodyDef;
    b2Body* body;
    private:

    b2PolygonShape box;
    b2FixtureDef fixtureDef;
    b2Fixture *fixture;
};
}
