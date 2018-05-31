#include "Script/cppmanager.hpp"

CPPManager CPPManager::instance;

void CPPManager::LoadLibrary()
{
    hndl = dlopen("../scriptCpp/libScriptCpp.so", RTLD_NOW);
}
Behaviour* CPPManager::InstantiateClass(const std::string &name)
{

    char *cstr = new char[name.length() + 1];
    strcpy(cstr, name.c_str());
    Behaviour* (*func)(char*) = (Behaviour* (*)(char*))dlsym(hndl, "Import");
    Behaviour* b = (*func)(cstr);
    delete cstr;
    return b;
}

