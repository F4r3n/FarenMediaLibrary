#include "interface.hpp"
#include <iostream>


maker_ptr Import(const char* name)
{
    return Factory::get().factory[name];
}
