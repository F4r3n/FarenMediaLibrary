#pragma once
#include "Script.h"
#include <map>
#include <memory>
namespace fm {

	class ScriptManager {
	public:
		ScriptManager();
		~ScriptManager();
		void init();
		void update(float dt);
		void start();
		void registerScript(const std::string &name);
		std::shared_ptr<Script> getScript(const std::string &name);
	private:
		std::map<std::string, std::shared_ptr<Script>> scripts;
		sol::state lua;
	};


}

