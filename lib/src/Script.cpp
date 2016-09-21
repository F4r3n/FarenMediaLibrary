#include "Script.h"
#include "ScriptRegister.h"
#include "CTransform.h"
using namespace fm;

std::string getFileName(const std::string& s) {

  char sep = '/';

#ifdef _WIN32
   sep = '\\';
#endif

   size_t i = s.rfind(sep, s.length());
   if (i != std::string::npos) {
      return(s.substr(i+1, s.length() - i - 5));
   }

   return("");
}

Script::Script(const std::string &name) {
    unsigned int value = ScriptRegister::addScript(name);
    this->scriptName = name;
    nameFile = getFileName(name);
    nameVariable = std::string("FM")+std::to_string(value)+std::string("_") + nameFile;
}



Script::Script() {}

Script::~Script() {

}

void Script::setName(const std::string &name) {
    this->scriptName = name;
    nameFile = getFileName(name);
    nameVariable = std::string("FM_") + nameFile;
}


void Script::start(sol::state &lua) {
  lua[nameVariable]["start"](lua[nameVariable]);
}

void Script::update(sol::state &lua, float dt) {

  lua[nameVariable]["update"](lua[nameVariable], dt);
  
}


std::string Script::getName() const {
    return scriptName;
}


bool Script::init(sol::state &lua, Entity *e){
	lua.script_file(scriptName);
  std::string m = std::string("f_") + nameFile;


  lua.script("local " + m + std::string(" = require '") + nameFile + std::string("'\n") 
              + nameVariable + std::string("= ") + m + std::string(".new()"));
    lua[nameVariable]["gameObject"] = e;
    Entity *e2 = lua[nameVariable]["gameObject"];
    std::cout << e2->get<fmc::CTransform>()->position.x << std::endl;
      return true;
}