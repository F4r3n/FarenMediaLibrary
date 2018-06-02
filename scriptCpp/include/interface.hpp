#pragma once
#include <map>
#include <string>
#include <iostream>
#include <Script/Script.h>

#if defined (WIN32) && defined (BUILD_SHARED_LIBS)
#if defined (_MSC_VER)
#pragma warning(disable: 4251)
#endif
   #if defined(MyLib_EXPORT)
     #define  MYLIB_EXPORT __declspec(dllexport)
   #else
     #define  MYLIB_EXPORT __declspec(dllimport)
   #endif
#else
  #define MYLIB_EXPORT
#endif

class ProxyTest
{
    public:
        ProxyTest();
};

typedef Behaviour* (*maker_ptr)();
class Factory
{
    public:
        Factory() {}
        inline static Factory& get() {
            return instance;
        }
        std::map <std::string, maker_ptr> factory;

    private:
        static Factory instance;
};


class Test : public Behaviour
{
    public:
        Test();
        virtual ~Test() {}
        void Update();
};
Behaviour *makerTest();


static void Export(const std::string& name, maker_ptr *ptr )
{
     Factory::get().factory[name] = *ptr;
}







extern "C"
{
    MYLIB_EXPORT maker_ptr Import(const char* name);
    MYLIB_EXPORT int test();
}

