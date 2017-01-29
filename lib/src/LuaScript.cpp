#include "LuaScript.h"
#include "ScriptRegister.h"
#include "Components/CTransform.h"
using namespace fm;

std::string getFileName(const std::string& s) {

    char sep = '/';

#ifdef _WIN32
    sep = '\\';
#endif

    size_t i = s.rfind(sep, s.length());
    if(i != std::string::npos) {
        return (s.substr(i + 1, s.length() - i - 5));
    }

    return ("");
}

LuaScript::LuaScript(const std::string& name, const std::string& nameVariable) {
    unsigned int value = ScriptRegister::addScript(name);
    this->scriptName = name;
    nameFile = getFileName(name);
    if(nameVariable == "")
        this->nameVariable = std::string("FM") + std::to_string(value) + std::string("_") + nameFile;
    else
        this->nameVariable = nameVariable;
}

LuaScript::LuaScript() {
}

LuaScript::~LuaScript() {
}

void LuaScript::setName(const std::string& name) {
    this->nameVariable = name;
}

void LuaScript::start() {
    LuaManager::get().getState()[nameVariable]["start"](LuaManager::get().getState()[nameVariable]);
}


void LuaScript::update() {

    LuaManager::get().getState()[nameVariable]["update"](LuaManager::get().getState()[nameVariable]);
}

std::string LuaScript::getName() const {
    return nameVariable;
}

bool LuaScript::init(Entity* e) {

    LuaManager::get().getState().script_file(scriptName);
    std::string m = std::string("f_") + nameFile;

    LuaManager::get().getState().script("local " + m + std::string(" = require '") + nameFile + std::string("'\n") + nameVariable +
               std::string("= ") + m + std::string(".new()"));
    LuaManager::get().getState()[nameVariable]["gameObject"] = e;
    for(auto o : objects) {
        LuaManager::get().getState()[nameVariable][o.first] = o.second;
    }
    return true;
}