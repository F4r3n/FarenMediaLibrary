#include "interface.hpp"
Factory Factory::instance;

Test::Test()
{
    std::cout << "INIIIIII" << std::endl;
}

void Test::Update()
{
    //std::cout << "yop" << std::endl;
}

Behaviour *makerTest()
{
   return new Test;
}

ProxyTest::ProxyTest()
{
    Factory::get().factory.emplace("Test", &makerTest);
    std::cout <<"B"<< Factory::get().factory.size() << std::endl;
}
static ProxyTest proxy;



