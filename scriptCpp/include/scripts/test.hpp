#ifndef TEST_HPP
#define TEST_HPP
#include "../interface.hpp"
#include <Components/CTransform.h>

class Test : public Behaviour
{
    public:
        Test();
        virtual ~Test() {}
        void Start();
        void Update();
    private:
        fmc::CTransform *transform;

};

#endif // TEST_HPP

