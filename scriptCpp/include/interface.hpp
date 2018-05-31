#pragma once
#include <map>
#include <string>
#include <iostream>
#include <Script/Script.h>


typedef Behaviour (*maker_ptr)();
static std::map <std::string, maker_ptr> factory;

class Test : public Behaviour
{
    public:
        Test();
        void Update()
        {
            std::cout << "yop" << std::endl;
        }


};

Behaviour *makerTest()
{
   return new Test;
}
class Proxy
{
    public:
        Proxy()
        {
            Export("Test", makerTest);
        }
};
Proxy p;


static void Export(const std::string& name, maker_ptr *ptr )
{
     factory[name] = ptr;
}

static Behaviour* Import(const char* name)
{
    return factory[name];
}

