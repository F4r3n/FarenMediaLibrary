#pragma once
#include "Script.h"
#include <map>
#include <memory>
#include <typeinfo>
#include "Component.h"

	class CTest : public Component<CTest>{
public:
	CTest() {}
	~CTest() {}
	float test = 0;
};
namespace fm {


	class ScriptManager {
	public:
		ScriptManager();
		~ScriptManager();
		void init();
		void update(float dt);
		void start();
		void registerScript(const std::string &name);

		template <typename T, typename ...Args>
		void registerComponent(const std::string &name, Args&&... args) {

	
			lua.new_usertype<T>(name, args...
             );
		}
		std::shared_ptr<Script> getScript(const std::string &name);

		template <typename T>
		T getComponent(const std::string nameScript, const std::string nameComponent) {
			return scripts[nameScript]->getComponent<T>(lua, nameComponent);
		}
	private:
		std::map<std::string, std::shared_ptr<Script>> scripts;
		sol::state lua;
	};


}

