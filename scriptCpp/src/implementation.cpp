#include "interface.hpp"
#include <iostream>
int test()
{
    return 2;
}

maker_ptr Import(const char* name)
{
    std::cout << "IMPORT" << std::endl;
    std::cout << Factory::get().factory.size() << std::endl;
    return Factory::get().factory[name];
}
