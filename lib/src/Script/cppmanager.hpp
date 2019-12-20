#ifndef CPPMANAGER_HPP
#define CPPMANAGER_HPP
#include "Script.h"
#if __linux__
#include <dlfcn.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

class CPPManager
{
    public:
        ~CPPManager() {}
        CPPManager() {}

        inline static CPPManager& get() {
            return instance;
        }
        void LoadPlugin();

        Behaviour* InstantiateClass(const std::string &name);


    private:

        static CPPManager instance;
#if __linux__
        void *hndl;
#else
		HINSTANCE hndl;
#endif 
};

#endif // CPPMANAGER_HPP

