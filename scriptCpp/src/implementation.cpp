#include "interface.hpp"
#include <iostream>
int test()
{
    return 2;
}

maker_ptr Import(const char* name)
{
    return Factory::get().factory[name];
}
