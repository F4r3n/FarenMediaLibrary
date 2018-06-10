#include "scripts/test.hpp"
#include <Time.h>
Test::Test()
{
    std::cout << "INIIIIII" << std::endl;
}

void Test::Start()
{
    transform = entity->get<fmc::CTransform>();
    transform->position.z += 1.0f;
}

void Test::Update()
{
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
