#include "scripts/test.hpp"
#include <TimeDef.h>
#include <Core/Debug.h>
#include <Entity.h>
Test::Test()
{
}

void Test::Start()
{
    transform = entity->get<fmc::CTransform>();
    transform->position.z += 1.0f;
}

void Test::Update()
{
    fm::Debug::get().LogError(std::to_string(fm::Time::dt));
    transform->rotation.y += 2.0f*fm::Time::dt;

}

Behaviour *makerTest()
{
   return new Test;
}
struct ProxyTest
{
        ProxyTest()
        {
            Factory::get().factory.emplace("Test", &makerTest);
        }
};

ProxyTest proxy;
