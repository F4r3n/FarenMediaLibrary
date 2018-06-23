#include "Script/cppmanager.hpp"
#include <cstring>
CPPManager CPPManager::instance;
typedef Behaviour* (*maker_ptr)();

void CPPManager::LoadLibrary()
{
    hndl = dlopen("../scriptCpp/libScriptCpp.so", RTLD_LAZY);
    if (!hndl)
    {
       fprintf(stderr, "Couldn't open lib: %s\n", dlerror());
     }
}
Behaviour* CPPManager::InstantiateClass(const std::string &name)
{
    const char *error;

    char *cstr = new char[name.length() + 1];
    strcpy(cstr, name.c_str());
    dlerror();
    maker_ptr (*func)(char*) = (maker_ptr (*)(char*))dlsym(hndl, "Import");
    if ((error = dlerror()))
    {
       fprintf(stderr, "Couldn't find import: %s\n", error);
       exit(1);
     }
    Behaviour* b = (*func)(cstr)();
    delete cstr;
    return b;
}

