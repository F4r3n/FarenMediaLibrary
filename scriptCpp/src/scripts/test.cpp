#include "scripts/test.hpp"
#include <Time.h>
Test::Test()
{
    std::cout << "INIIIIII" << std::endl;
}

void Test::Start()
{
    transform = entity->get<fmc::CTransform>();
}

void Test::Update()
{
    transform->position.x += 0.1f*fm::Time::dt;

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
