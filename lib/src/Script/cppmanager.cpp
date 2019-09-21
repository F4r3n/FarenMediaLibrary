#include "Script/cppmanager.hpp"
#include <cstring>
#include <iostream>


CPPManager CPPManager::instance;
typedef Behaviour* (*maker_ptr)();

void CPPManager::LoadPlugin()
{

#if __linux__
	hndl = dlopen("../scriptCpp/libScriptCpp.so", RTLD_LAZY);
	if (!hndl)
	{
		fprintf(stderr, "Couldn't open lib: %s\n", dlerror());
	}
#else
	hndl = LoadLibrary("../scriptCpp/Debug/ScriptCpp.dll");
	if (!hndl) {
		std::cout << "could not load the dynamic library" << std::endl;
	}
#endif

}
Behaviour* CPPManager::InstantiateClass(const std::string &name)
{
    const char *error = '\0';

    char *cstr = new char[name.length() + 1];
    strcpy(cstr, name.c_str());

#if __linux__
	maker_ptr(*func)(char*) = (maker_ptr(*)(char*))dlsym(hndl, "Import");
	if ((error = dlerror()))
	{
		fprintf(stderr, "Couldn't find import: %s\n", error);
		exit(1);
}
#else
	maker_ptr(*func)(char*) = (maker_ptr(*)(char*))GetProcAddress(hndl, "Import");
	if (!(*func)(cstr))
	{
		fprintf(stderr, "Couldn't find import: %s\n", error);
		exit(1);
	}
#endif
   

    Behaviour* b = (*func)(cstr)();
    delete cstr;
    return b;
}

