#pragma once
#include "component.h"
#include <Box2D/Box2D.h>
#include "../Core/Math/Vector2.h"
#include <string>
namespace fmc {

class Body2D : public FMComponent<Body2D> {
   public:
    Body2D(unsigned int w, unsigned int h, bool isDynamic = false);
    Body2D();

    ~Body2D();

    void Init(b2World* world, float P2M);

    void StartContact();
    void EndContact();
    void SetFriction(float friction);

    void ApplyForceCenter(fm::math::Vector2f power);
    void ApplyForce(fm::math::Vector2f&& power, fm::math::Vector2f&& pos);
    void ApplyForceCenter2(float x, float y);

    float density = 0.0f;
    float friction = 0.0f;
    int number_contact = 0;
    size_t* identity = nullptr;
    fm::math::Vector2f size;
    bool isDynamic = false;

    static const std::string name;

    bool isReady = false;
    b2BodyDef bodyDef;
    b2Body* body;

   private:
    b2PolygonShape box;
    b2FixtureDef fixtureDef;
    b2Fixture* fixture;
};
}
