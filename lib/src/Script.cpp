#include "Script.h"
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
    this->scriptName = name;
    nameFile = getFileName(name);
    nameVariable = std::string("FM_") + nameFile;
    std::cout << nameVariable << std::endl;
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
  lua[nameVariable]["start"]();
  transform.position.x = lua[nameVariable]["components"]["transform"]["position"]["x"];
  transform.position.y = lua[nameVariable]["components"]["transform"]["position"]["y"];
   
}

void Script::update(sol::state &lua, float dt) {

  lua[nameVariable]["update"](lua[nameVariable], dt);

  transform.position.x = lua[nameVariable]["components"]["transform"]["position"]["x"];
  transform.position.y = lua[nameVariable]["components"]["transform"]["position"]["y"];
  
}




Transform Script::getTransform() const {
	return transform;
}

std::string Script::getName() const {
    return scriptName;
}




bool Script::init(sol::state &lua){
	lua.script_file(scriptName);
  std::string m = std::string("f_") + nameFile;


  lua.script("local " + m + std::string(" = require '") + nameFile + std::string("'\n") 
        + nameVariable + std::string("= ") + m + std::string(".new()"));

      return true;
}