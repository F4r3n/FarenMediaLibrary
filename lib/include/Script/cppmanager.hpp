#ifndef CPPMANAGER_HPP
#define CPPMANAGER_HPP
#include "Script.h"
//TODO if unix
 #include <dlfcn.h>

class CPPManager
{
    public:
        ~CPPManager() {}
        CPPManager() {}

        inline static CPPManager& get() {
            return instance;
        }
        void LoadLibrary();

        Behaviour* InstantiateClass(const std::string &name);


    private:

        static CPPManager instance;
        void *hndl;
};

#endif // CPPMANAGER_HPP

